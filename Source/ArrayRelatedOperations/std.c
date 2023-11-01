#include <math.h>

double std(int data[], int n, float mean)
{
    double variance = 0.0;

    for(int i=0; i<n; ++i)
        variance += (data[i] - mean) * (data[i] - mean);

    return sqrt(variance/n);
}
