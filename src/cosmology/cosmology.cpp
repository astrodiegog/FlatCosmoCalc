#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "../cosmology/cosmology.h"
#include "../cosmology/cosmoparams.h"


Cosmology::Cosmology(struct CosmoParams *cosmoparams_in, struct TimeDomainParams *timedomain_in)
{
    cosmoparams = cosmoparams_in;
    timedomainparams = timedomain_in;

    /* Calculate rho_crit */
    rho_crit = 3. * (cosmoparams->H0) * (cosmoparams->H0)/(8. * PI * G_CGS);
    
    z_infty = 1.e12;
    
    integral_acc = 1e-14;
    integral_Rmmax = 14;

    ndata_table = 0;

#ifdef SAVE_TIME_LOOKBACK
    ndata_table++;
#endif

#ifdef SAVE_TIME_AGE
    ndata_table++;
#endif

#ifdef SAVE_TIME_CONFORMAL
    ndata_table++;
#endif

#ifdef SAVE_DIST_LUM
    ndata_table++;
#endif

#ifdef SAVE_DIST_COMOVING_LOS
    ndata_table++;
#endif

#ifdef SAVE_DIST_COMOVING_TRANS
    ndata_table++;
#endif

#ifdef SAVE_DIST_ANG_DIAM
    ndata_table++;
#endif

#ifdef SAVE_DIST_MOD
    ndata_table++;
#endif

#ifdef SAVE_HUBBLE
    ndata_table++;
#endif

#ifdef SAVE_DENSITY
    ndata_table++;
#endif

}

double *Cosmology::GetTimeArray()
{
    /* Declare iterator */
    int i;

    /* Dynamically allocate memory for time array */
    double *time_array = (double *) malloc((timedomainparams->ntime) * sizeof(double));

    if(!timedomainparams->zmin)
    {
        /* Have to be careful with a minimum redshift of 0 */
        timedomainparams->zmin = 1e-6;
    }

    if(timedomainparams->log)
    {
        double l_zmin = log10(1 + timedomainparams->zmin);
        double l_zmax = log10(1 + timedomainparams->zmax);
        double l_dz = (l_zmax - l_zmin)/(timedomainparams->ntime - 1);

        for (i = 0;i<timedomainparams->ntime;i++)
        {
            double l_z = l_zmin + i*l_dz;
            time_array[i] = pow(10, l_z);
        }

    }
    else
    {
        double dz = ((timedomainparams->zmax) - (timedomainparams->zmin))/(timedomainparams->ntime - 1);

        for (i=0;i<timedomainparams->ntime;i++)
        {
            time_array[i] = (timedomainparams->zmin) + i*dz;
        }
    }

    return time_array;
}

void Cosmology::PrintCosmoParams()
{
    printf(" --- Cosmological Parameters --- \n");
    printf("\t H0: %.3e (1/s) = %.3f (km/s/Mpc) \n", cosmoparams->H0, (cosmoparams->H0)/(KM_CGS/MPC_CGS));
    printf("\t OmegaM: %.3e \n", cosmoparams->OmegaM);
    printf("\t OmegaR: %.3e \n", cosmoparams->OmegaR);
    printf("\t OmegaL: %.3e \n", cosmoparams->OmegaL);
    printf("\t OmegaK: %.3e \n", cosmoparams->OmegaK);
    printf("\t w0: %.3e \n", cosmoparams->w0);
    printf("\t wa: %.3e \n", cosmoparams->wa);
    printf("\t rho_crit: %.3e (g/cm^3) \n", rho_crit);
}

void Cosmology::PrintTimeDomainParams()
{
    printf(" --- Time Domain Info --- \n");
    printf("\t zmin: %.4f \t amin: %.4e \n", timedomainparams->zmin, timedomainparams->amin);
    printf("\t zmax: %.4f \t amax: %.4e \n", timedomainparams->zmax, timedomainparams->amax);
    printf("\t Log-Spaced? %d \n", timedomainparams->log);
    printf("\t Number of data points %d \n", timedomainparams->ntime);
}


double **Cosmology::GetIntegrationTable()
{
    /* Declare CosmoSnap */
    struct CosmoSnapshot cosmosnap;

    /* Declare iterator for each redshift and datapoint to save */
    int i, j;

    /* Declare the integral table as array of double pointers */
    double **int_table = (double **) malloc(timedomainparams->ntime * sizeof(double *));

    /* Grab redshift array */
    double *time_array = GetTimeArray();

    for (i = 0; i < timedomainparams->ntime; i++)
    {
        j = 0;
        
        double curr_redshift = time_array[i];
        
        /* Dynamically allocate memory for this row's data */
        int_table[i] = (double *) malloc((3 + ndata_table) * sizeof(double));

        cosmosnap.z_snap = curr_redshift;
        cosmosnap.l_z_snap = log10(1. + curr_redshift);
        cosmosnap.a_snap = 1. / (1. + cosmosnap.z_snap);

        int_table[i][0] = cosmosnap.z_snap;
        int_table[i][1] = cosmosnap.l_z_snap;
        int_table[i][2] = cosmosnap.a_snap;

    #ifdef SAVE_TIME_LOOKBACK
        cosmosnap.time_lookback = time_lookback(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.time_lookback;
        j++;
    #endif

    #ifdef SAVE_TIME_AGE
        cosmosnap.time_age = time_age(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.time_age;
        j++;
    #endif

    #ifdef SAVE_TIME_CONFORMAL
        cosmosnap.time_conformal = time_conformal(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.time_conformal;
        j++;
    #endif

    #ifdef SAVE_DIST_LUM
        cosmosnap.dist_lum = dist_luminosity(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.dist_lum / MPC_CGS;
        j++;
    #endif

    #ifdef SAVE_DIST_COMOVING_LOS
        cosmosnap.dist_comoving_los = dist_comoving_LOS(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.dist_comoving_los / MPC_CGS;
        j++;
    #endif

    #ifdef SAVE_DIST_COMOVING_TRANS
        cosmosnap.dist_comoving_trans = dist_comoving_trans(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.dist_comoving_trans / MPC_CGS;
        j++;
    #endif

    #ifdef SAVE_DIST_ANG_DIAM
        cosmosnap.dist_angdiam = dist_angular_diam(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.dist_angdiam / MPC_CGS;
        j++;
    #endif

    #ifdef SAVE_DIST_MOD
        cosmosnap.dist_mod = dist_modulus(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.dist_mod;
        j++;
    #endif

    #ifdef SAVE_HUBBLE
        cosmosnap.hubble = hubble_param(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.hubble / (KM_CGS / MPC_CGS);
        j++;
    #endif

    #ifdef SAVE_DENSITY
        cosmosnap.density = density(cosmosnap.z_snap);
        int_table[i][j+3] = cosmosnap.density;
        j++;
    #endif
    }

    /* De-allocate memory for redshift array */
    free(time_array);

    return int_table;
}


void Cosmology::SaveIntegrationTable(double **integral_table, char *fname)
{
    /* Declare outgoing integration table file name and pointer */
    FILE *foutptr;

    /* Declare iterator for each redshift and datapoint */
    int i, j;

    /* Create file name */
    foutptr = fopen(fname, "w");

    if (foutptr == NULL)
    {
        printf("Couldn't open file, oops! \n");
    }
    else
    {
    #ifdef DEBUG
        printf("Writing outputs !\n");
    #endif

        /* Place cosmological parameters */
        fprintf(foutptr, " --- Cosmological Parameters --- \n");
        fprintf(foutptr, "\t H0: %.4e (1/s) = %.4f (km/s/Mpc) \n", cosmoparams->H0, (cosmoparams->H0)/(KM_CGS/MPC_CGS));
        fprintf(foutptr, "\t OmegaM: %.4e \n", cosmoparams->OmegaM);
        fprintf(foutptr, "\t OmegaR: %.4e \n", cosmoparams->OmegaR);
        fprintf(foutptr, "\t OmegaL: %.4e \n", cosmoparams->OmegaL);
        fprintf(foutptr, "\t OmegaK: %.4e \n", cosmoparams->OmegaK);
        fprintf(foutptr, "\t\t w0: %.4e \n", cosmoparams->w0);
        fprintf(foutptr, "\t\t wa: %.4e \n", cosmoparams->wa);
        fprintf(foutptr, "\t rho_crit: %.4e (g/cm^3) \n", rho_crit);


        /* Place age of cosmology to present day */
        double age_s = time_age(0.) / cosmoparams->H0;
        double age_Gyr = age_s / GYR_CGS;
        printf("Age of Universe: %.6e s = %.6e Gyr\n", age_s, age_Gyr);


        /* Place header of data into file */
        fprintf(foutptr, "z, log10(1+z), a");

    #ifdef SAVE_TIME_LOOKBACK
        fprintf(foutptr, ", H0*t_look");
    #endif

    #ifdef SAVE_TIME_AGE
        fprintf(foutptr, ", H0*t_age");
    #endif

    #ifdef SAVE_TIME_CONFORMAL
        fprintf(foutptr, ", H0*t_conf");
    #endif

    #ifdef SAVE_DIST_LUM
        fprintf(foutptr, ", d_L");
    #endif

    #ifdef SAVE_DIST_COMOVING_LOS
        fprintf(foutptr, ", d_LOS");
    #endif

    #ifdef SAVE_DIST_COMOVING_TRANS
        fprintf(foutptr, ", d_trans");
    #endif

    #ifdef SAVE_DIST_ANG_DIAM
        fprintf(foutptr, ", d_ang");
    #endif

    #ifdef SAVE_DIST_MOD
        fprintf(foutptr, ", d_mod");
    #endif

    #ifdef SAVE_HUBBLE
        fprintf(foutptr, ", H(z)");
    #endif

    #ifdef SAVE_DENSITY
        fprintf(foutptr, ", rho_c(z)");
    #endif

        fprintf(foutptr, " \n");


        /* Place data into file */
        for (i = 0; i < timedomainparams->ntime; i++)
        {
            fprintf(foutptr, "%.8e, %.8e, %.8e", integral_table[i][0], integral_table[i][1], integral_table[i][2]);

            for (j = 0; j < ndata_table; j++)
            {
                fprintf(foutptr, ", %.8e", integral_table[i][j+3]);
            }

            fprintf(foutptr, "\n");
        }
    }

    fclose(foutptr);

}

 

void Cosmology::DestroyIntegrationTable(double **integral_table)
{
    /* Declare iterator for each redshift */
    int i;

    /* Free each integral table row */
    for (i = 0; i < timedomainparams->ntime ; i++)
    {
        free(integral_table[i]);
    }
    free(integral_table);
}
