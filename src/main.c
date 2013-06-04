#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "dd.h"

#include "config.h"
#include "kiss_fft.h"
#include "audio-io.h"
#include "piano-fft.h"
#include "utils.h"

#define TIMER_ID 30
#define TIMER_VALUE 100

WAVFILE audioFile;
int done = 0;
int samplesRead = 0;
long totalSamplesRead = 0;
FFT_RESULT fftResult;
long long lastFFTTime;
long long lastLogTime;
int iterations = 0;


void HeartBeat();

/* Globals */
char      g_szAppName[] = "DDSamp";
HWND      g_hwndMain;
HINSTANCE g_hInstance;
bool      g_bRunning;


LRESULT CALLBACK WndProc(
    HWND   hWnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam )
{
    switch ( msg ) {
        case WM_PAINT: {
            /* Let Windows know we've redrawn the Window - since we've bypassed
               the GDI, Windows can't figure that out by itself. */
            ValidateRect( hWnd, NULL );
        }
        break;

        case WM_DESTROY:
            g_bRunning = false;
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc( hWnd, msg, wParam, lParam);
    }

    return 0;
}

WAVFILE setupAudio(char *filename)
{
    if (!OpenWavFile(filename, &audioFile)) {
        printf("Audio file opened successfully.\r\n");
    } else {
        printf("Error opening audio file.\r\n");
        exit(EXIT_FAILURE);
    }
    return audioFile;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wce;
    wce.cbSize        = sizeof(wce);
    wce.style         = CS_VREDRAW | CS_HREDRAW;
    wce.lpfnWndProc   = (WNDPROC) WndProc;
    wce.cbClsExtra    = 0;
    wce.cbWndExtra    = 0;
    wce.hInstance     = hInstance;
    wce.hIcon         = LoadIcon((HINSTANCE) NULL, IDI_APPLICATION);
    wce.hCursor       = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
    wce.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wce.lpszMenuName  = 0;
    wce.lpszClassName = "ADPWinClass",
        wce.hIconSm       = 0;

    if (!RegisterClassEx(&wce)) {
        return 0;
    }

    HWND hWnd = CreateWindowEx(
                    0,              /* Ex Styles */
                    "ADPWinClass",
                    "ADP GmbH",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT,  /* x */
                    CW_USEDEFAULT,  /* y */
                    CW_USEDEFAULT,  /* Height */
                    CW_USEDEFAULT,  /* Width */
                    NULL,           /* Parent Window */
                    NULL,           /* Menu, or windows id if child */
                    hInstance,      /*  */
                    NULL            /*  Pointer to window specific data */
                );

    /* Initialize DirectDraw */
    if (!DDInit( hWnd )) {
        MessageBox( hWnd, "Failed to initialize DirectDraw", "Error", MB_OK );
        return 0;
    }

    /* Create DirectDraw surfaces */
    if (!DDCreateSurfaces( false )) {
        MessageBox( hWnd, "Failed to create surfaces", "Error", MB_OK );
        return 0;
    }

    ShowWindow( hWnd, nCmdShow );
    setupAudio(AUDIO_FILE);
    lastFFTTime = milliseconds_now();
    lastLogTime = milliseconds_now();
    MSG msg;
    int r;
    /* Invalidate window area so that it gets redrawn first time round  */
    InvalidateRect( hWnd, NULL, TRUE );
    g_bRunning = true;

    while (g_bRunning) {
        while (PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE)) {
            BOOL bGetResult = GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (bGetResult == 0) {
                g_bRunning = false;
            }
        }

        if (g_bRunning) {
            CheckSurfaces();
            HeartBeat();
        }
    }

    while ((r = GetMessage(&msg, NULL, 0, 0 )) != 0) {
        if (r == -1) {
            ;  // Error!
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CloseWavFile(&audioFile);
    /* The application's return value  */
    return msg.wParam;
}

void HeartBeat()
{
    int i;  
    int ms = (int) milliseconds_now() - lastFFTTime;

    if (ms < 33) {
        return;
    }

    lastFFTTime = milliseconds_now();

    if (fftResult.bins != NULL) {
        free(fftResult.bins);
        fftResult.bins = NULL;
    }

    samplesRead = 0;    
    kiss_fft_cpx *in = LoadSamples(&audioFile, ms, &samplesRead, 16384);
    totalSamplesRead += samplesRead;
    iterations++;  
     
    kiss_fft_cpx *result = malloc(samplesRead * sizeof(kiss_fft_cpx));
    fftResult = PerformFFT(in, result, samplesRead);
    fftResult.wavFile = &audioFile;
    free(in);
    ms = (int) milliseconds_now() - lastLogTime;

    if (ms > 1000) {
        lastLogTime = milliseconds_now();
        float avBins = (totalSamplesRead / (float) iterations) / 2.0;
        float nyquist = audioFile.header.SampleRate / 2.0;
        float binres = nyquist / avBins;
        printf("Log (1000ms) av bins: %f, nyquist: %f hz, bin res: %f \n", avBins, nyquist, binres );
        iterations = 0;
        totalSamplesRead = 0;
    }

    /* Check for lost surfaces  */
    CheckSurfaces();
    /* Clear the back buffer */
    DDClear( g_pDDSBack, 0, 0, 320, 240 );
    HDC hDC;
    IDirectDrawSurface_GetDC(g_pDDSBack, &hDC);
    RECT r = getKeyboardRect();
    HBRUSH frameBrush =
        CreateSolidBrush(
            RGB(
                0,
                0,
                0
            )
        );
    FrameRect(hDC, &r, frameBrush);

    for (i = 0; i < KEY_COUNT; i++) {
        RECT r2 = getKeyRect(i);
        HBRUSH b = getKeyBrush(&fftResult, i);
        FillRect(hDC, &r2, b);
        DeleteObject(b);
        /* FrameRect(hDC, &r2, frameBrush); */
    }

    DeleteObject(frameBrush);
    IDirectDrawSurface_ReleaseDC(g_pDDSBack, hDC);
    /* Blit the back buffer to the front buffer */
    DDFlip();
}
