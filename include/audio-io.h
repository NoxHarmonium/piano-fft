/*
* audio-io.h
* 
* Defines the structs and functions related to audio file 
* access.
*
*/

#ifndef AUDIO_IO_H
#define AUDIO_IO_H

/* Structs */

typedef struct {
    int32_t  ChunkID;
    int32_t  ChunkSize;
    int32_t  Format;
    int32_t  Subchunk1ID;
    int32_t  Subchunk1Size;
    int16_t  AudioFormat;
    int16_t  NumChannels;
    int32_t  SampleRate;
    int32_t  ByteRate;
    int16_t  BlockAlign;
    int16_t  BitsPerSample;
    int32_t  Subchunk2ID;
    int32_t  Subchunk2Size;

} WAVFILE_HEADER ;

typedef struct {
    int valid;
    FILE *file;
    WAVFILE_HEADER header;
    unsigned char* buffer;    
    unsigned char* window_pos;
    unsigned char* buffer_pos;

} WAVFILE ;


/* Function Prototypes */

int OpenWavFile(char *filename, WAVFILE *wavFile);
int CloseWavFile(WAVFILE *wavFile);
kiss_fft_cpx *LoadSamples(WAVFILE *wavFile, int millis, int *samplesRead, int windowSize);


#endif
