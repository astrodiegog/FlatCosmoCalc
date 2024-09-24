#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../integrator/integrator.h"
#include "../integrator/integral_test.h"
#include "../integrator/integrands_test.h"

void RunAllTests()
{
    /* Declare struct holding integration values */
    struct IntegratorTestInfo integratortest_info;

    /* Declare integration table */
    double **int_table;

    /* Declare integrand argument arrays & pointers */
    double integrand_args_one[1];
    double integrand_args_two[2];
    double *pintegrand_args_one = &integrand_args_one[0];
    double *pintegrand_args_two = &integrand_args_two[0];

    /* Declare file name for each integrand test */
    char fileName1[MAXLEN] = "integrandONE_vals.txt";
    char fileName2[MAXLEN] = "integrandTWO_vals.txt";
    char fileName3[MAXLEN] = "integrandTHREE_vals.txt";
    char fileName4[MAXLEN] = "integrandFOUR_vals.txt";
    char fileName4LN[MAXLEN] = "integrandFOURLN_vals.txt";
    char fileName5[MAXLEN] = "integrandFIVE_vals.txt";
    char fileName5LN[MAXLEN] = "integrandFIVELN_vals.txt";


    /* Declare & initialize n-th order of Romberg integration, number of integrals, and accuracy */
    integratortest_info.nmax = 12;
    integratortest_info.nints = 50;
    integratortest_info.acc = 0.0000000001;

    /*
    ** Test 1 ! 
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 1.0;
    integratortest_info.log = false;
    strncpy(integratortest_info.outFile, fileName1, MAXLEN);
    integrand_args_one[0] = 0.;

    /* Create integration table */
    int_table = CreateIntegralTable(&integratortest_info, integrandONE, pintegrand_args_one); 

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);



    /* 
    ** Test 2 ! 
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 1.57079632679;
    integratortest_info.log = false;
    strncpy(integratortest_info.outFile, fileName2, MAXLEN);
    integrand_args_one[0] = 0.;

    /* Create integration table */
    int_table = CreateIntegralTable(&integratortest_info, integrandTWO, pintegrand_args_one);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);



    /* 
    ** Test 3 ! (test argument pass-through)
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 1.0;
    integratortest_info.log = false;
    strncpy(integratortest_info.outFile, fileName3, MAXLEN);
    integrand_args_two[0] = 1.;
    integrand_args_two[1] = -2.;

    /* Create integration table */
    int_table = CreateIntegralTable(&integratortest_info, integrandTHREE, pintegrand_args_two);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);



    /* 
    ** Test 4 ! 
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 1.0;
    integratortest_info.log = false;
    strncpy(integratortest_info.outFile, fileName4, MAXLEN);
    integrand_args_one[0] = 2.;

    /* Create integration table */
    int_table = CreateIntegralTable(&integratortest_info, integrandFOUR, pintegrand_args_one);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);


    /* 
    ** Test 5 ! (testing log-space integration)
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 10000.0;
    integratortest_info.log = true;
    strncpy(integratortest_info.outFile, fileName4LN, MAXLEN);
    integrand_args_one[0] = 2.;

    /* Create log-space integration table */
    int_table = CreateIntegralTableLN(&integratortest_info, integrandFOUR_LN, pintegrand_args_one);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);



    /* 
    ** Test 6 ! (arctan)
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 1.0;
    integratortest_info.log = false;
    strncpy(integratortest_info.outFile, fileName5, MAXLEN);
    integrand_args_one[0] = 2.;

    /* Create integration table */
    int_table = CreateIntegralTable(&integratortest_info, integrandFIVE, pintegrand_args_one);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);


    /* 
    ** Test 7 ! (testing log-space integration)
    */
    integratortest_info.a = 0.0;
    integratortest_info.b = 10000.0;
    integratortest_info.log = true;
    strncpy(integratortest_info.outFile, fileName5LN, MAXLEN);
    integrand_args_one[0] = 2.;

    /* Create log-space integration table */
    int_table = CreateIntegralTableLN(&integratortest_info, integrandFIVE_LN, pintegrand_args_one);

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);




}


void RunTestFile(char *integralparam_file, double (*integrand)(double, double*), double *integrand_args)
{
    /* Declare struct holding integration values */
    struct IntegratorTestInfo integratortest_info;

    /* Declare integration table */
    double **int_table;

    /* Grab info from integralparam_file & place onto IntegratorTestInfo */
    Parse_IntegralParams(integralparam_file, &integratortest_info);

#ifdef DEBUG
    printf("Param Info: \n \ta = %.4f, b = %.4f, nmax = %.2i nint = %.2i \n \tacc = %.4e \n \toutfile = %s\n", integratortest_info.a, integratortest_info.b, integratortest_info.nmax, integratortest_info.nints, integratortest_info.acc, integratortest_info.outFile);
#endif

    /* Create integration table */
    if (integratortest_info.log)
    {
        int_table = CreateIntegralTableLN(&integratortest_info, integrand,integrand_args);
    }
    else
    {
        int_table = CreateIntegralTable(&integratortest_info, integrand,integrand_args);
    }

    /* Save integration table */
    SaveIntegralTable(&integratortest_info, int_table);

    /* Free the integration table from memory */
    DestroyIntegralTable(&integratortest_info, int_table);
}

void SaveIntegralTable(struct IntegratorTestInfo *integratortestinfo, double **integral_table)
{
    /* Declare outgoing integration table file pointer */
    FILE *foutptr;

    /* iterator */
    int i;

    foutptr = fopen(integratortestinfo->outFile, "w");
    if (foutptr == NULL)
    {
        printf("Couldn't open file, oops! \n");
    }
    else
    {
    #ifdef DEBUG
        printf("Writing outputs !\n");
    #endif
        /* Place header into file */
        fprintf(foutptr, "a = %.20f \n", integratortestinfo->a);
        fprintf(foutptr, "b = %.20f \n", integratortestinfo->b);
        fprintf(foutptr, "nMAX = %4d \n", integratortestinfo->nmax);
        fprintf(foutptr, "expected accuracy = %.20f \n", integratortestinfo->acc);
        fprintf(foutptr, "number of integrations = %4d \n", integratortestinfo->nints);

        if (integratortestinfo->log)
        {
            fprintf(foutptr, "ln(x), F(x) \n");
        }
        else
        {
            fprintf(foutptr, "x, F(x) \n");
        }
        
        for (i=0; i < (integratortestinfo->nints - 1); i++)
        {
            fprintf(foutptr, "%.20e, %.20e \n", integral_table[i][0], integral_table[i][1]);
        }
    }
}



void DestroyIntegralTable(struct IntegratorTestInfo *integratortestinfo, double **integral_table)
{
    /* iterator */
    int i;

    for (i=0; i<(integratortestinfo->nints-1); i++)
    {
        free(integral_table[i]);
    }

    free(integral_table);
}

double **CreateIntegralTableLN(struct IntegratorTestInfo *integratortestinfo, double (*integrandXLN)(double, double*), double *integrandXLN_args)
{
    /* iterator */
    int i;

    /* Declare integral, integral limits, and step size */
    double integral, ln_x_upp, ln_xmin, ln_xmax, ln_h;

    /* Declare + Define number of rows to save */
    int nrows = integratortestinfo->nints - 1;

    /* Declare the integral table as array of double pointers */
    double **int_table = (double **) malloc(nrows * sizeof(double *));

    /* Prevent negative or zero for integration bounds */
    if(integratortestinfo->a <= 0.)
    {
        integratortestinfo->a = 1e-20;
    }
    
    if(integratortestinfo->b <= 0.)
    {
        integratortestinfo->b = 1e-5;
    }

    /* Set lower and upper integration bounds in log-space */
    ln_xmin = log(integratortestinfo->a);
    ln_xmax = log(integratortestinfo->b);

    /* Calculate the step size over desired integration bounds */
    ln_h = (ln_xmax - ln_xmin) / ((double)nrows) ;

    for (i=0; i < nrows; i++)
    {
        /* Calculate the upper limit to integrate out to */
        ln_x_upp = ln_xmin + (double)(i+1) * ln_h;

        /* Compute the integral */
        integral = RombergIntegral(integrandXLN, integrandXLN_args, ln_xmin, 
        ln_x_upp, integratortestinfo->nmax, integratortestinfo->acc);

        /* Declare memory for this row's data*/
        int_table[i] = (double *) malloc(2 * sizeof(double));

        /* Place data onto integral table */
        int_table[i][0] = ln_x_upp;
        int_table[i][1] = integral;

    #ifdef DEBUG
        printf("Integral %i (%.4e -> %.4e): %.8f \n", i, integratortestinfo->a, exp(ln_x_upp), integral);
        printf("\t ln(a): %.4e \t ln(x): %.4e \t x: %.4e \n\n", ln_xmin, ln_x_upp, exp(ln_x_upp));
    #endif
    }
    
    return int_table;
}


double **CreateIntegralTable(struct IntegratorTestInfo *integratortestinfo, double (*integrand)(double, double*), double *integrand_args)
{
    /* iterator */
    int i;

    /* Declare integral, upper integral lim, and step size */
    double integral, x_upp, h;

    /* Declare + Define number of rows to save */
    int nrows = integratortestinfo->nints - 1;

    /* Declare the integral table as array of double pointers */
    double **int_table = (double **) malloc(nrows * sizeof(double *));

    /* Calculate the step size over desired integration bounds */
    h = (integratortestinfo->b - integratortestinfo->a)/((double)nrows) ;

    for (i=0; i < nrows; i++)
    {
        /* Calculate the upper limit to integrate out to */
        x_upp = integratortestinfo->a + (double)(i+1) * h;

        /* Compute the integral */
        integral = RombergIntegral(integrand, integrand_args, integratortestinfo->a, 
        x_upp, integratortestinfo->nmax, integratortestinfo->acc);

        /* Declare memory for this row's data*/
        int_table[i] = (double *) malloc(2 * sizeof(double));

        /* Place data onto integral table */
        int_table[i][0] = x_upp;
        int_table[i][1] = integral;

    #ifdef DEBUG
        printf("Integral %i (%.4e -> %.4e): %.8f\n\n", i, integratortestinfo->a, x_upp, integral);
    #endif
    }
    
    return int_table;
}



extern void Parse_IntegralParam(char *key, char *value, struct IntegratorTestInfo *integratortestinfo)
{
    if(!strcmp(key, "a"))
    {
        integratortestinfo->a = atof(value);
    }
    else if (!strcmp(key, "b"))
    {
        integratortestinfo->b = atof(value);
    }
    else if (!strcmp(key, "nmax"))
    {
        integratortestinfo->nmax = atoi(value);
    }
    else if (!strcmp(key, "acc"))
    {
        integratortestinfo->acc = atof(value);
    }
    else if (!strcmp(key, "nints"))
    {
        integratortestinfo->nints = atof(value);
    }
    else if (!strcmp(key, "log"))
    {
        if (value[0] == 'Y')
        {
            integratortestinfo->log = true;
        }
        else if (value[0] == 'N')
        {
            integratortestinfo->log = false;
        }
        else
        {
            printf("UNKNOWN LOG PARAM: %s = %s", key, value);
        }
    }
    else if (!strcmp(key, "outFile"))
    {
        strncpy(integratortestinfo->outFile, value, MAXLEN);
    }
    else
    {
        printf("UNKNOWN PARAMETER: %s = %s", key, value);
    }
}

extern void Parse_IntegralParams(char *integralparam_file, struct IntegratorTestInfo *integratortestinfo)
{
    /* Declare buffer array */
    char buff[MAXLEN];

    /* Declare pointer to pointer array */
    char *str;

    /* Declare pointer to integral param file*/
    FILE *fptr;

    /* Key-Value pairs to read in param values*/
    char key[MAXLEN], value[MAXLEN];

    fptr = fopen(integralparam_file, "r");

    if (fptr == NULL)
    {
        printf("Couldn't open file, oops! \n");
    }

    while ((str = fgets(buff, MAXLEN, fptr)) != NULL)
    {
        /* Place output token onto str */
        str = strtok(buff, "=");

        /* Grab key */
        if (str)
        {
            /* Copy token str onto key array */
            strncpy(key, str, MAXLEN);
        }
        else
        {
            continue;
        }

        /* Continue tokenizing */
        str = strtok(NULL, "=");

        /* Grab value */
        if(str)
        {
            /* Copy token str onto value array */
            strncpy(value, str, MAXLEN);
        }
        else
        {
            continue;
        }

        /* Place key-value char* into IntegratorTestInfo struct */
        Parse_IntegralParam(key, value, integratortestinfo);
        
    }

    fclose(fptr);
}
