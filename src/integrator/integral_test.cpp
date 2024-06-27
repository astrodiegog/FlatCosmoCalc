#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../integrator/integrator.h"
#include "../integrator/integral_test.h"
#include "../integrator/integrands.h"


void RunTest(char *integralparam_file, double (*integrand)(double, double*), double *integrand_args)
{
    /* Declare struct holding integration values */
    struct IntegratorTestInfo integratortest_info;

    /* Grab info from integralparam_file & place onto IntegratorTestInfo */
    Parse_IntegralParams(integralparam_file, &integratortest_info);

#ifdef DEBUG
    printf("Param Info: \n \ta = %.4f, b = %.4f, nmax = %.2i nint = %.2i \n \tacc = %.4e \n \toutfile = %s\n", integratortest_info.a, integratortest_info.b, integratortest_info.nmax, integratortest_info.nints, integratortest_info.acc, integratortest_info.outFile);
#endif

    /* Create integration table */
    double **int_table = CreateIntegralTable(&integratortest_info, integrand,integrand_args);

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

        fprintf(foutptr, "x, F(x) \n");
        for (i=0; i < (integratortestinfo->nints - 1); i++)
        {
            fprintf(foutptr, "%.20f, %.20f \n", integral_table[i][0], integral_table[i][1]);
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
