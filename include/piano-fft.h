#ifndef FFT_LEDS_H
#define FFT_LEDS_H

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif

/* Structs */

typedef struct {
    kiss_fft_cpx *bins;
    int samples;
    WAVFILE *wavFile;
} FFT_RESULT ;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} FFT_RECT ;

typedef struct {
	int r;
	int g;
	int b;
} FFT_BRUSH ;

/* Function Prototypes */

FFT_RECT getKeyboardRect();
FFT_RECT getKeyRect(int keyIndex);
FFT_BRUSH getKeyBrush(FFT_RESULT *result, int keyIndex);
FFT_RESULT PerformFFT(kiss_fft_cpx *in, kiss_fft_cpx *out, int samplesRead);
void ApplyWindowing(kiss_fft_cpx* samples, int samplesRead);

#endif
