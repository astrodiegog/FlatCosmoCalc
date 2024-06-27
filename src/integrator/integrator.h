/*\fn double pow(double, int)*/
/* Routine to calculate base number to integer exponent */
double my_pow(double base, int exponent);

/*\fn double fabs(double)*/
/* Routine to return absolute value of a double */
double my_fabs(double num);

/*\fn double R_m0_sum(double*, double, double, int)*/
/* Routine to calculate sum in R(n,0) term for Romberg integration */
double R_m0_sum(double (*f)(double), double *fn_args, double a, double b, int n);

/*\fn double RombergIntegral(double*, double, double, int, double)*/
/* Routine to calcluate definite integral using Romberg's method */
double RombergIntegral(double (*f)(double, double*), double *fn_args, double a, double b, int n_max, double acc);
