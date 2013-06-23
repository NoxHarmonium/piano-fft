# Thanks: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

#CC=clang
CFLAGS= -g -ansi -pedantic -Wall -std=c99 -I$(IDIR) -I/usr/include/SDL
SDL_CFLAGS= `sdl-config --cflags`
IDIR=include
ODIR=obj
LDIR=lib
SDIR=src
MAINFILE=main.sdl.o

LIBS=-lSDLmain -lSDL -lm
SDL_LIBS=`sdl-config --libs`

_DEPS = _kiss_fft_guts.h  audio-io.h  dd.h  piano-fft.h  kiss_fft.h  utils.h config.h window-funcs.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = audio-io.o  kiss_fft.o  piano-fft.o utils.o window-funcs.o  $(MAINFILE) 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)	$(SDL_CFLAGS)
    
piano-fft: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(SDL_LIBS)
    
all: style piano-fft


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(SDIR)/*.orig    
    
style:
	astyle --style=ansi -C -S -N -f  -p -H -k1 -W1 -j -c -v src/*.c
