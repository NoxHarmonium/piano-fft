# Thanks: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=gcc
CFLAGS= -g -ansi -pedantic -Wall -std=c99 -I$(IDIR) 
IDIR=include
ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm -mwindows -lddraw

_DEPS = _kiss_fft_guts.h  audio-io.h  dd.h  piano-fft.h  kiss_fft.h  utils.h config.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = audio-io.o  dd.o  kiss_fft.o  main.o  piano-fft.o  utils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))



$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)	
    
piano-fft: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
    
all: style piano-fft

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(SDIR)/*.orig    
    
style:
	astyle --style=ansi -C -S -N -f  -p -H -k3 -W1 -j -c -v src/*.c