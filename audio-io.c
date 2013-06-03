#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "kiss_fft.h"
#include "audio-io.h"


// Opens the WAV file, populates the HEADER struct and does some basic sanity checking.
// Also cues the file to the data.
// Using https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
int OpenWavFile(char *filename, WAVFILE *wavFile)
{
    printf("Attempting to open: '%s'\r\n", filename);

    //WAVFILE _wavFile = *wavFile;
    wavFile->valid = 0;

    wavFile->file = fopen(filename, "r");

    if (wavFile->file == NULL) {
        printf("Unable to open file: %s\r\n", filename);
        return 1;
    }

    WAVFILE_HEADER header;

    // Read in header data.
    size_t length = fread(&header, sizeof(header), 1, wavFile->file);

    if (length != 1) {
        printf("Unable to read header data. Length: %i Expected: %i\r\n", length, 1);
        return 1;
    }

    if (header.ChunkID != *(int32_t *) "RIFF") {
        printf("Unable to read standard WAV header. ChunkID: 0x%x Expected: 0x%x \r\n", header.ChunkID, *(int32_t *) "RIFF");
        return 1;
    }

    if (header.Format != *(int32_t *) "WAVE") {
        printf("Format is not 'WAVE'.\r\n");
        return 1;
    }

    if (header.AudioFormat != 1) {
        printf("Only PCM is supported at this time.\r\n");
        return 1;
    }

    wavFile->header = header;

    printf("Sample rate: %i, NumChannels: %i, BitsPerSample: %i\r\n", wavFile->header.SampleRate, wavFile->header.NumChannels, wavFile->header.BitsPerSample);

    wavFile->valid = 1;

    // Basic sanity checks passed. Continue.
    fseek(wavFile->file, HEADER_LENGTH, SEEK_SET);

    printf("WAV file opened successfully.\r\n");
    return 0;
}

kiss_fft_cpx *LoadSamples(WAVFILE *wavFile, int millis, int *samplesRead)
{
    int i = 0;
    int bytesRead = 0;
    int _samplesRead = 0;
    

    int samples = wavFile->header.SampleRate * ((double)millis / 1000.0);
    int kissSamples = kiss_fft_next_fast_size(samples);
    int bytesPerSample = wavFile->header.BitsPerSample / 8;
    kiss_fft_cpx *values = malloc(kissSamples * sizeof(kiss_fft_cpx));
    unsigned char *buffer = malloc(bytesPerSample * sizeof(unsigned char) * samples);

    //printf("Samples: %i, bytesPerSample: %i, sizeof(kiss_fft_cpx): %i\r\n", samples, bytesPerSample, sizeof(kiss_fft_cpx));

    int maxValue = 0;

    for (i = 0; i < bytesPerSample; i++) {
        maxValue |= 0xFF << i * 8;
    }



    //printf("maxValue: %i \r\n", maxValue);

    bytesRead = fread(buffer, sizeof(unsigned char), bytesPerSample * samples, wavFile->file);

    //printf("bytesRead: %i\r\n", bytesRead);

    if (bytesRead == 0) {

        return NULL;
    }



    i = 0;

    while (i < samples) {
        int j;
        int sample = 0;
        unsigned char *converter = calloc(sizeof(int), 1);




        for (j = 0; j < bytesPerSample; j++) {
            converter[j] = buffer[i + j];
        }

        sample = *((int *)converter);

        //printf("%x,",sample);


        kiss_fft_cpx s;
        s.r = ((kiss_fft_scalar)sample) / (kiss_fft_scalar) maxValue;

        //printf("%x = %f,",sample,s.r);

        assert(s.r <= 1.0);

        s.i = 0;

        i += bytesPerSample;

        if (_samplesRead > samples) {
            printf("Samples read (%i) > samples allocated (%i)! \r\n", _samplesRead, samples);
            exit(EXIT_FAILURE);
        }

        values[_samplesRead++] = s;

        

        free(converter);
    }

    kiss_fft_cpx empty;
    for (i = _samplesRead; i < kissSamples; i++)
    {
        values[i] = empty;
    }

    *samplesRead = kissSamples;

    free(buffer);
    return values;
}

int CloseWavFile(WAVFILE *wavFile)
{
    printf("Closing WAV file...\r\n");
    fclose(wavFile->file);
    wavFile->valid = 0;
    return 0;
}
