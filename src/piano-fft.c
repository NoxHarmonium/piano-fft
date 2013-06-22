#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "config.h"
#include "kiss_fft.h"
#include "audio-io.h"
#include "piano-fft.h"
#include "utils.h"
#include "window-funcs.h"

/* 
    Piano Key Frequency Table
    A list of all the frequencies of the white notes on a piano.
*/
const float pFreqTable[52] =
{
    27.5, 30.8677, 32.7032, 36.7081, 41.2034, 43.6535, 48.9994,
    55, 61.7354, 65.4064, 73.4162, 82.4069, 87.3071, 97.9989,
    110, 123.471, 130.813, 146.832, 164.814, 174.614, 195.998,
    220, 246.942, 261.626, 293.665, 329.628, 349.228,  391.995,
    440, 493.883, 523.251, 587.33, 659.255, 698.456,  783.991,
    880, 987.767, 1046.5, 1174.66,  1318.51,  1396.91, 1567.98,
    1760,  1975.53, 2093, 2349.32, 2637.02, 2793.83, 3135.96,
    3520, 3951.07, 4186.01
};

/* An array of magnitudes to allow for smoothing */
kiss_fft_scalar magnitudes[52];

FFT_RECT getKeyboardRect()
{
    FFT_RECT r =
    {
        KEYBOARD_XOFFSET,
        KEYBOARD_YOFFSET,
        KEYBOARD_WIDTH,
        KEYBOARD_HEIGHT
    };
    return r;
}

FFT_RECT getKeyRect(int keyIndex)
{
    int left = KEYBOARD_XOFFSET + (keyIndex * KEY_WIDTH);
    FFT_RECT r =
    {
        left,
        KEYBOARD_YOFFSET,
        KEY_WIDTH,
        KEYBOARD_HEIGHT
    };
    return r;
}

FFT_BRUSH createBrush(int r, int g, int b)
{
	FFT_BRUSH brush;
	brush.r = r;
	brush.g = g;
	brush.b = b;
	return brush;
}

FFT_BRUSH getKeyBrush(FFT_RESULT* result, int keyIndex)
{
    float r = 0;
    float g = 0;
    float b = 0;
	
    if (result->wavFile == NULL)
    {
        return createBrush((int)r, (int)g, (int)b);
    }

    float f = pFreqTable[keyIndex];
    int sampleRate = result->wavFile->header.SampleRate;
    int samples = result->samples;
    assert(sampleRate != 0);
    float fPerBin = sampleRate / (float)samples;
    int binIndex = (int) round(f / fPerBin);
    int maxBinIndex = (samples / 2) + 1;

    if (binIndex > maxBinIndex || binIndex == 0)
    {
        return createBrush((int)r, (int)g, (int)b);
    }

    kiss_fft_scalar value = sqrt(pow(result->bins[binIndex].r, 2) + pow(result->bins[binIndex].i, 2));
    magnitudes[keyIndex] = fmax(value, magnitudes[keyIndex]);
    float h = 0;
    float s = 1;
    float v = 1;
    h = magnitudes[keyIndex] / ((kiss_fft_scalar)samples / 2) * 360;
    magnitudes[keyIndex] *= 0.90;
    HSVtoRGB( &r, &g, &b, h, s, v );
	
    return createBrush((int)floor(r * 254),(int)floor(g * 254),(int)floor(b * 254));
}

void ApplyWindowing(kiss_fft_cpx* samples, int samplesRead)
{
    int i;
    
    for (i = 0; i < samplesRead; i++)
    {
        float mult = WF_Hanning(i, samplesRead);
        samples[i] *= mult;
    }


}

FFT_RESULT PerformFFT(kiss_fft_cpx* in, kiss_fft_cpx* out, int samplesRead)
{
    kiss_fft_cfg cfg;    
    
    if (samplesRead > 0)
    {
        /* printf("Samples Read: %i \n", samplesRead);  */
        if ((cfg = kiss_fft_alloc(samplesRead, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
        {
            kiss_fft(cfg, in, out);
            free(cfg);
        }
        else
        {
            printf("Cannot set up kissFFT.\n");
            exit(EXIT_FAILURE);
        }
    }

    FFT_RESULT result;
    result.bins = out;
    result.samples = samplesRead;
    return result;
}
