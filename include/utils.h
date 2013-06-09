#ifndef FFT_LED_UTILS_H
#define FFT_LED_UTILS_H

long long milliseconds_now();
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
char* WCharToChar(LPWSTR s);

#endif
