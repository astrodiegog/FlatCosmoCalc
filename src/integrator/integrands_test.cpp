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
    double a_x = (a + x);
    double a_x2 = a_x * (a + x);
    double a_x3 = a_x2 * (a + x);
    double a_x4 = a_x3 * (a + x);

    return 1. / (a_x4);
}

double integrandFOUR_LN(double ln_x, double *args)
{
    double x = exp(ln_x);
    double integrand = integrandFOUR(x, args);

    return integrand * x;
}

double integrandFIVE(double x, double *args)
{
    double a = args[0];
    double a2 = a * a ;
    double x2 = x * x ;

    return 1. / (a2 + x2);
}

double integrandFIVE_LN(double ln_x, double *args)
{
    double x = exp(ln_x);
    double integrand = integrandFIVE(x, args);

    return integrand * x;
}

