/*\fn void Parse_IntegralParams(char *, struct IntegratorTestInfo) */
/* Routine to place info from param file onto IntegratorTestInfo struct */
extern void Parse_IntegralParams(char *integralparam_file, struct IntegratorTestInfo *integratortestinfo);

/*\fn void Parse_IntegralParam(char *, char *, struct* IntegratorTestInfo) */
/* Routine to place key-value pair onto IntegratorTestInfo struct */
extern void Parse_IntegralParam(char *key, char *value, struct IntegratorTestInfo *integratortestinfo);

/*\fn double **CreateIntegralTable(struct* IntegratorTestInfo, double*, double*) */
/* Routine to create integration table and populate */
double **CreateIntegralTable(struct IntegratorTestInfo *integratortestinfo, double (*integrand)(double, double*), double *integrand_args);

/*\fn void DestroyIntegralTable(struct* IntegratorTestInfo, double**) */
/* Routine to free integration table from memory */
void DestroyIntegralTable(struct IntegratorTestInfo *integratortestinfo, double **integral_table);

/*\fn void SaveIntegralTable(struct *IntegratorTestInfo, double **) */
/* Routine to save Integral Table to file specified in IntegratorTestInfo */
void SaveIntegralTable(struct IntegratorTestInfo *integratortestinfo, double **integral_table);

/*\fn void RunTest(char *, double*, double*) */
/* Routine to save integral table on integrand to where integralparamfile says */
void RunTest(char *integralparam_file, double (*integrand)(double, double*), double *integrand_args);

/* Define max string buffer lengths */
#define MAXLEN 1028

/* Define info needed to test Romberg integrator */
struct IntegratorTestInfo
{
    float a;                // lower limit of integration
    float b;                // upper limit of integration
    int nmax;               // maximum n-th order of Romberg integration
    int nints;              // number of integral performances between a and b
    float acc;              // accuracy to end integration
    char outFile[MAXLEN];   // where to place outgoing integration table
};
