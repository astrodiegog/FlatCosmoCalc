#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cosmology/cosmoparams.h"
#include "../cosmology/cosmology.h"


extern void Parse_CosmoParam(char *key, char *value, struct CosmoParams *cosmoparams)
{
    if(!strcmp(key, "H0"))
    {
        cosmoparams->H0 = (double) atof(value);
    }
    else if (!strcmp(key, "OmegaM"))
    {
        cosmoparams->OmegaM = (double) atof(value);
    }
    else if (!strcmp(key, "OmegaR"))
    {
        cosmoparams->OmegaR = (double) atof(value);
    }
    else if (!strcmp(key, "OmegaK"))
    {
        cosmoparams->OmegaK = (double) atof(value);
    }
    else if (!strcmp(key, "w0"))
    {
        cosmoparams->w0 = (double) atof(value);
    }
    else if (!strcmp(key, "wa"))
    {
        cosmoparams->wa = (double) atof(value);
    }
    else
    {
        printf("UNKNOWN PARAMETER: %s = %s", key, value);
    }
}

extern void Parse_CosmoParams(char *cosmoparam_file, struct CosmoParams *cosmoparams)
{
    /* Declare buffer array */
    char buff[MAXLEN];

    /* Declare pointer to pointer array */
    char *str;

    /* Declare pointer to cosmology param file */
    FILE *fptr;

    /* Key-Value pairs to read in param values */
    char key[MAXLEN], value[MAXLEN];

    fptr = fopen(cosmoparam_file, "r");

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

        /* Place key-value char* into CosmoParams struct */
        Parse_CosmoParam(key, value, cosmoparams);
        
    }

    fclose(fptr);


    /* Set OmegaDE */
    cosmoparams->OmegaL = 1. - (cosmoparams->OmegaK + cosmoparams->OmegaR + cosmoparams->OmegaM);    

    /* Convert H0 (km/s/Mpc) to (1/s)*/
    cosmoparams->H0 /= MPC_CGS;
    cosmoparams->H0 *= KM_CGS;
}


extern void Parse_TimeParam(char *key, char *value, struct TimeDomainParams *timedomain)
{
    if(!strcmp(key, "zmin"))
    {
        timedomain->zmin = (double) atof(value);
    }
    else if (!strcmp(key, "zmax"))
    {
        timedomain->zmax = (double) atof(value);
    }
    else if (!strcmp(key, "ntime"))
    {
        timedomain->ntime = atoi(value);
    }
    else if (!strcmp(key, "log"))
    {
        if (value[0] == 'Y')
        {
            timedomain->log = true;
        }
        else if (value[0] == 'N')
        {
            timedomain->log = false;
        }
        else
        {
            printf("UNKNOWN LOG PARAM: %s = %s", key, value);
        }
    }
    else
    {
        printf("UNKNOWN PARAMETER: %s = %s", key, value);
    }
}

extern void Parse_TimeParams(char *timedomainparam_file, struct TimeDomainParams *timedomain)
{
    /* Declare buffer array */
    char buff[MAXLEN];

    /* Declare pointer to pointer array */
    char *str;

    /* Declare pointer to time domain param file */
    FILE *fptr;

    /* Key-Value pairs to read in param values */
    char key[MAXLEN], value[MAXLEN];

    fptr = fopen(timedomainparam_file, "r");

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

        /* Place key-value char* into TimeDomainParams struct */
        Parse_TimeParam(key, value, timedomain);
        
    }

    fclose(fptr);

    /* Set amin & amax */
    timedomain->amin = 1. / (1. + timedomain->zmin);
    timedomain->amax = 1. / (1. + timedomain->zmax);
}
