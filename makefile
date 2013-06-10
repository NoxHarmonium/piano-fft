# Thanks: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

#CC=clang
CFLAGS= -g -ansi -pedantic -Wall -std=c99 -I$(IDIR) -I/usr/include/SDL
IDIR=include
ODIR=obj
LDIR=lib
SDIR=src
MAINFILE=main.sdl.o

LIBS=-lm -lSDL

_DEPS = _kiss_fft_guts.h  audio-io.h  dd.h  piano-fft.h  kiss_fft.h  utils.h config.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = audio-io.o  kiss_fft.o  piano-fft.o  utils.o $(MAINFILE)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))



$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)	
    
piano-fft: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
    
all: style piano-fft


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(SDIR)/*.orig    
    
style:
	astyle --style=ansi -C -S -N -f  -p -H -k1 -W1 -j -c -v src/*.c
