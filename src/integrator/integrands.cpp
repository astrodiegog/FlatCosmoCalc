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

double integrandFOUR(double x, double *args)
{
    double a = args[0];
    double one_x = (1. + x);
    double one_x2 = one_x * (1. + x);
    double one_x3 = one_x2 * (1. + x);

    return 1. / sqrt(a * one_x3);
}

double integrandFOUR_LN(double ln_x, double *args)
{
    double x = exp(ln_x);
    double integrand = integrandFOUR(x, args);

    return integrand * x;
}
