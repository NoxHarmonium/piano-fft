#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"


void setupScreen()
{
	SDL_Surface *screen;

    screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit); /* Call SDL_QUIT when program exits normally */


	

}
