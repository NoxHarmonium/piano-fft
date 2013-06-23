#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

#include "config.h"
#include "kiss_fft.h"
#include "audio-io.h"
#include "piano-fft.h"
#include "utils.h"

SDL_Surface* screen;
WAVFILE audioFile;
int done = 0;
int samplesRead = 0;
long totalSamplesRead = 0;
FFT_RESULT fftResult;
Uint32 lastFFTTime;
Uint32 lastLogTime;
int iterations = 0;
int running = true;
SDL_Event event;

WAVFILE setupAudio(char* filename)
{
    if (!OpenWavFile(filename, &audioFile))
    {
        printf("Audio file opened successfully.\r\n");
    }
    else
    {
        printf("Error opening audio file.\r\n");
        exit(EXIT_FAILURE);
    }

    return audioFile;
}

void setupScreen()
{

    screen = SDL_SetVideoMode(KEYBOARD_WIDTH, KEYBOARD_HEIGHT, 16, SDL_SWSURFACE);

    if ( screen == NULL )
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void FillRect(FFT_RECT r, FFT_BRUSH b)
{
    int color = SDL_MapRGB(screen->format, b.r, b.b, b.g);

    SDL_Rect rect = {r.x, r.y, r.w, r.h};
    SDL_FillRect(screen, &rect, color);
}

/*
void DrawRect(FFT_RECT r, FFT_BRUSH b) {
    int color = SDL_MapRGB(screen->format,b.r,b.b,b.g);

    SDL_Rect rect = {r.x,r.y,r.w,r.h};
    SDL_DrawRect(screen, &rect, color);
}
*/

void ClearScreen()
{
    /* This function just fills a surface with a given color. The //
    // first 0 tells SDL to fill the whole surface. The second 0  //
    // is for black. */


    SDL_FillRect(screen, 0, 0);

}


void HeartBeat()
{
    int i;
    int ms = (int) SDL_GetTicks() - lastFFTTime;

    lastFFTTime = SDL_GetTicks();

    if (fftResult.bins != NULL)
    {
        free(fftResult.bins);
        fftResult.bins = NULL;
    }

    samplesRead = 0;

    AdvanceWindow(&audioFile, ms);
    LoadBuffer(&audioFile);
    kiss_fft_cpx* in = LoadSamples(&audioFile, ms, &samplesRead, WINDOW_SIZE);


    totalSamplesRead += samplesRead;
    iterations++;

    ApplyWindowing(in, samplesRead);
    kiss_fft_cpx* result = malloc(samplesRead * sizeof(kiss_fft_cpx));
    fftResult = PerformFFT(in, result, samplesRead);
    fftResult.wavFile = &audioFile;
    free(in);
    ms = (int) SDL_GetTicks() - lastLogTime;

    if (ms > 1000)
    {
        lastLogTime = SDL_GetTicks();
        float secondsPlayed = totalSamplesRead / (float) audioFile.header.SampleRate;
        float avBins = (totalSamplesRead / (float) iterations) / 2.0;
        float nyquist = audioFile.header.SampleRate / 2.0;
        float binres = nyquist / avBins;
        printf("Log (1000ms) av bins: %f, nyquist: %f hz, bin res: %f s: %f \n", avBins, nyquist, binres, secondsPlayed);
        iterations = 0;
        totalSamplesRead = 0;
    }




    FFT_RECT r = getKeyboardRect();
    FFT_BRUSH frameColor =
    {
        0,
        0,
        0
    };

    SDL_Flip(screen);
    ClearScreen();

    FillRect(r, frameColor);

    for (i = 0; i < KEY_COUNT; i++)
    {
        FFT_RECT r2 = getKeyRect(i);
        FFT_BRUSH b = getKeyBrush(&fftResult, i);
        FillRect(r2, b);
    }




}

int main(int argc, char* argv[])
{
    if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 )
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    atexit(SDL_Quit); /* Call SDL_QUIT when program exits normally */

    lastLogTime = SDL_GetTicks();
    lastFFTTime = SDL_GetTicks();

    setupScreen();
    /* TODO: Basic argument sanity checking */
    setupAudio(argv[1]);

    while (running)
    {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        HeartBeat();
        SDL_Delay(1000 / FRAME_RATE);
    }




    return EXIT_SUCCESS;
}
