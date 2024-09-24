/*\fn double integrandONE(double)*/
/* Routine to help unit test integrator. Integrate 0->1 for PI */
double integrandONE(double x, double *args);

/*\fn double integrandTWO(double)*/
/* Routine to help unit test integrator. Compare against sin(x) */
double integrandTWO(double x, double *args);

/*\fn double integrandTHREE(double)*/
/* Routine to help unit test integrator. Compare against ln(ax+b)/a */
double integrandTHREE(double x, double *args);

/*\fn double integrandFOUR(double)*/
/* Routine to help unit test integrator. Compare against -1/(3 x^3) */
double integrandFOUR(double x, double *args);

/*\fn double integrandFOUR_LN(double)*/
/* Routine to help unit test integrator on logspace. Calls integrandFOUR */
double integrandFOUR_LN(double x, double *args);

/*\fn double integrandFIVE(double)*/
/* Routine to help unit test integrator. Compare against arctan(x/a)/a */
double integrandFIVE(double x, double *args);

/*\fn double integrandFIVE_LN(double)*/
/* Routine to help unit test integrator on logspace. Calls integrandFIVE */
double integrandFIVE_LN(double x, double *args);

