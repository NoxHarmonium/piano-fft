#ifndef FFT_LEDS_H
#define FFT_LEDS_H

#define KEYBOARD_WIDTH 200  
#define KEYBOARD_HEIGHT 50
#define KEY_COUNT 52
#define KEY_WIDTH (KEYBOARD_WIDTH / KEY_COUNT)

#define KEYBOARD_XOFFSET 50
#define KEYBOARD_YOFFSET 50

const float pFreqTable[52];

typedef struct {
    kiss_fft_cpx *bins;
    int samples;
    WAVFILE *wavFile;
} FFT_RESULT ;


RECT getKeyboardRect();
RECT getKeyRect(int keyIndex);
HBRUSH getKeyBrush(FFT_RESULT *result, int keyIndex);
FFT_RESULT PerformFFT(kiss_fft_cpx *in, kiss_fft_cpx *out, int samplesRead);

#endif
