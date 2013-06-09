#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "config.h"
#include "kiss_fft.h"
#include "audio-io.h"

/*
 Opens the WAV file, populates the HEADER struct and does some basic sanity checking.
 Also cues the file to the data.
 Thanks: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
*/
int OpenWavFile(char* filename, WAVFILE* wavFile)
{
    WAVFILE_HEADER header;
    size_t length;
    int bytesRead;
    unsigned char* buffer_ptr;

    printf("Attempting to open: '%s'\r\n", filename);

    wavFile->valid = 0;
    wavFile->file = fopen(filename, "r");

    if (wavFile->file == NULL)
    {
        printf("Unable to open file: %s\r\n", filename);
        return 1;
    }


    /* Read in header data. */
    length = fread(&header, sizeof(header), 1, wavFile->file);

    if (length != 1)
    {
        printf("Unable to read header data. Length: %i Expected: %i\r\n", length, 1);
        return 1;
    }

    if (header.ChunkID != *(int32_t*) "RIFF")
    {
        printf("Unable to read standard WAV header. ChunkID: 0x%x Expected: 0x%x \r\n", header.ChunkID, *(int32_t*) "RIFF");
        return 1;
    }

    if (header.Format != *(int32_t*) "WAVE")
    {
        printf("Format is not 'WAVE'.\r\n");
        return 1;
    }

    if (header.AudioFormat != 1)
    {
        printf("Only PCM is supported at this time.\r\n");
        return 1;
    }

    wavFile->header = header;
    printf("Sample rate: %i, NumChannels: %i, BitsPerSample: %i\r\n", wavFile->header.SampleRate, wavFile->header.NumChannels, wavFile->header.BitsPerSample);
    wavFile->valid = 1;
    /* Basic sanity checks passed. Continue. */
    fseek(wavFile->file, HEADER_LENGTH, SEEK_SET);
    wavFile->buffer = malloc(BUFFER_SIZE);     

    /* Load initial buffer */
    LoadBuffer(wavFile);
  

    printf("WAV file opened successfully.\r\n");
    return 0;
}

/* Advance a pointer with wrapping. */
unsigned char* BufferAdvance(unsigned char* position, unsigned char* offset, int windowSize)
{
    unsigned char* result = position + offset;
    result = result % (BUFFER_SIZE - windowSize);      
    
    return result;    
}

void LoadBuffer(WAVFILE* wavFile)
{
    /* Try to align the file buffer on the opposite side of the circular buffer to the window */
    unsigned char* desiredPosition = BufferAdvance(wavFile->window_pos, BUFFER_SIZE / 2, CHUNK_SIZE);
    if (wavFile->buffer_pos >= desiredPosition)
        return;
    
    wavFile->buffer_pos = BufferAdvance(wavFile->buffer_pos, CHUNK_SIZE, CHUNK_SIZE);
    
    bytesRead = fread(wavFile->buffer + wavFile->buffer_pos, sizeof(unsigned char), BUFFER_CHUNK, wavFile->file);
    wavFile->buffer_pos = bytesRead; 
    

}

void AdvanceWindow(WAVFILE* wavFile, int ms)
{
    int samples = wavFile->header.SampleRate * ((double)millis / 1000.0);
    wavFile->window_pos = BufferAdvance(wavFile->window_pos, samples, WINDOW_SIZE);   

}

kiss_fft_cpx* LoadSamples(WAVFILE* wavFile, int millis, int* samplesRead, int windowSize)
{
    kiss_fft_cpx* values;
    int i = 0;
    int bytesRead = 0;
    int _samplesRead = 0;
    int maxValue = 0;
    int samples = wavFile->header.SampleRate * ((double)millis / 1000.0);
    int kissSamples = kiss_fft_next_fast_size(windowSize);
    int bytesPerSample = wavFile->header.BitsPerSample / 8;
    unsigned char* buffer_pos;
    kiss_fft_cpx empty;

    wavFile->buffer += samples * bytesPerSample;

    if ((int)wavFile->buffer > wavFile->max_buffer_pos)
    {
        printf("buffer (%p) > buffer length (%x) !\n", wavFile->buffer, wavFile->max_buffer_pos);
        exit(1); /* EOB */
    }

    values = malloc(kissSamples * sizeof(kiss_fft_cpx));

    for (i = 0; i < bytesPerSample; i++)
    {
        maxValue |= 0xFF << i * 8;
    }


    bytesRead = windowSize;

    if (bytesRead == 0)
    {
        return NULL;
    }

    i = 0;

    buffer_pos = wavFile->buffer;

    while (i < kissSamples)
    {
        kiss_fft_cpx s;
        int j;
        int sample = 0;
        unsigned char* converter = calloc(sizeof(int), 1);

        for (j = 0; j < bytesPerSample; j++)
        {
            converter[j] = buffer_pos[i + j];
        }

        sample = *((int*)converter);

        s.r = ((kiss_fft_scalar)sample) / (kiss_fft_scalar) maxValue;

        assert(s.r <= 1.0);
        s.i = 0;
        i += bytesPerSample;

        if (_samplesRead > kissSamples)
        {
            printf("Samples read (%i) > samples allocated (%i)! \r\n", _samplesRead, samples);
            exit(EXIT_FAILURE);
        }

        values[_samplesRead++] = s;
        free(converter);
    }



    for (i = _samplesRead; i < kissSamples; i++)
    {
        values[i] = empty;
    }

    *samplesRead = kissSamples;
    return values;
}

int CloseWavFile(WAVFILE* wavFile)
{
    printf("Closing WAV file...\r\n");
    fclose(wavFile->file);
    wavFile->valid = 0;
    return 0;
}
