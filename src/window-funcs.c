#include <math.h>
#include "window-funcs.h"

const double pi = 3.141592653589793238462643383279502884197169399375105820974944;

float WF_Rectagle(int sample, int totalSamples)
{
    return 1.0;
}

float WF_Hanning(int sample, int totalSamples)
{
    /* http://en.wikipedia.org/wiki/Window_function#Hann_.28Hanning.29_window */
    return 0.5 *
           (1 - cos(
                (2.0 * pi * sample) /
                sample
            )
           );
}
