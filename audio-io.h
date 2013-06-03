#ifndef AUDIO_IO_H
#define AUDIO_IO_H

#define HEADER_LENGTH 44
#define AUDIO_FILE "./rise.wav"

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
    

} WAVFILE ;




int OpenWavFile(char *filename, WAVFILE *wavFile);
int CloseWavFile(WAVFILE *wavFile);
kiss_fft_cpx *LoadSamples(WAVFILE *wavFile, int millis, int *samplesRead);


#endif
