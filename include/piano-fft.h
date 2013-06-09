#ifndef FFT_LEDS_H
#define FFT_LEDS_H

/* Structs */

typedef struct {
    kiss_fft_cpx *bins;
    int samples;
    WAVFILE *wavFile;
} FFT_RESULT ;

/* Function Prototypes */

RECT getKeyboardRect();
RECT getKeyRect(int keyIndex);
HBRUSH getKeyBrush(FFT_RESULT *result, int keyIndex);
FFT_RESULT PerformFFT(kiss_fft_cpx *in, kiss_fft_cpx *out, int samplesRead);

#endif
