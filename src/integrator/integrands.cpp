#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double integrandONE(double x, double *args)
{
    return (4.) / (1. + x * x);
}

double integrandTWO(double x, double *args)
{
    return cos(x);
}

double integrandTHREE(double x, double *args)
{
    double a = args[0];
    double b = args[1];

    return 1. / (a * x + b);
}
