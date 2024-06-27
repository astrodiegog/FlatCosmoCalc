/*\fn double integrandONE(double)*/
/* Routine to help unit test integrator. Integrate 0->1 for PI */
double integrandONE(double x, double *args);

/*\fn double integrandTWO(double)*/
/* Routine to help unit test integrator. Compare against sin(x) */
double integrandTWO(double x, double *args);

/*\fn double integrandTWO_exp(double)*/
/* Routine to help unit test integrator. Compare against ln(ax+b)/a */
double integrandTHREE(double x, double *args);

