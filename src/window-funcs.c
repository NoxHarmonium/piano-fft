#define _USE_MATH_DEFINES
#include <math.h>
#include "window-funcs.h"

float WF_Hanning(int sample, int totalSamples)
{    
    /* http://en.wikipedia.org/wiki/Window_function#Hann_.28Hanning.29_window */
    return 0.5 * 
    (1 - cos(
        (2.0 * M_PI * sample) / 
        sample
        )
    );
}