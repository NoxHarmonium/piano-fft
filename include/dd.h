/*--------------------------------------------------------------------------*/
#ifndef _DD_H_
#define _DD_H_
/*--------------------------------------------------------------------------*/
#include <ddraw.h>

typedef int bool;
#define false 0
#define true 1

/*--------------------------------------------------------------------------
   Variables
--------------------------------------------------------------------------*/
LPDIRECTDRAW        g_pDD;        /* The DirectDraw object */
LPDIRECTDRAWCLIPPER g_pClipper;   /* Clipper for primary surface */
LPDIRECTDRAWSURFACE g_pDDS;       /* Primary surface */
LPDIRECTDRAWSURFACE g_pDDSBack;   /* Back surface */
HWND                g_hWnd;       /* To store the main windows handle */
bool                g_bFullScreen; /* Full-screen mode? */

int                 g_iBpp;     /* Remember the main surface bit depth */
/*--------------------------------------------------------------------------
   Functions
--------------------------------------------------------------------------*/

/*-- Housekeeping */

/* Initialize basic DirectDraw stuff */
bool DDInit( HWND hWnd );
/* Create surfaces */
bool DDCreateSurfaces( bool bFullScreen);
/* Destroy surfaces */
void DDDestroySurfaces();
/* Clean up DirectDraw stuff */
void DDDone();
/* Checks if the memory associated with surfaces is lost and restores if necessary. */
void CheckSurfaces();


/*-- Drawing */

/* PutPixel routine for a DirectDraw surface */
void DDPutPixel( LPDIRECTDRAWSURFACE pDDS, int x, int y, int r, int g, int b );
/* Create color from RGB triple */
unsigned int CreateRGB( int r, int g, int b );
/* Clear a surface area with black */
void DDClear( LPDIRECTDRAWSURFACE pDDS, int x1, int y1, int x2, int y2 );
/* Double buffering flip */
void DDFlip();

/*--- Error checking stuff */
bool  DDFailedCheck(HRESULT hr, char *szMessage);
char *DDErrorString(HRESULT hr);

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/
