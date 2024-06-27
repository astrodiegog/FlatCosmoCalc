#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "../cosmology/cosmology.h"
#include "../cosmology/cosmoparams.h"
#include "../integrator/integrator.h"


double time_evo(double z, double *args)
{
    double OmegaM0 = args[0];
    double OmegaR0 = args[1];
    double OmegaL0 = args[2];
    double OmegaK0 = args[3];
    double w0 = args[4];
    double wa = args[5];

    double one_z = 1+z;
    double one_z2 = one_z * one_z;
    double one_z3 = one_z * one_z2;
    double one_z4 = one_z * one_z3;
    double one_zDE = pow(one_z3, 1. + w0 + wa);
    double exp_DE = exp((-3. * wa) * (z / one_z));

    double O_k = (OmegaK0)*one_z2;
    double O_M = (OmegaM0)*one_z3;
    double O_R = (OmegaR0)*one_z4;
    double O_L = (OmegaL0)*one_zDE*exp_DE;

    return sqrt(O_M + O_R + O_L + O_k);
}


double dist_comovingLOS_integrand(double z, double *args)
{
    return 1. / (time_evo(z, args));
}

double Cosmology::dist_comoving_LOS(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];

    /* Calculate hubble distance */
    double DH = C_CGS/cosmoparams->H0;

    if (z)
    {
        return DH*RombergIntegral(dist_comovingLOS_integrand, fn_args, 0., z, integral_Rmmax, integral_acc);
    }
    else
    {
        return 0;
    }
}


double Cosmology::dist_comoving_trans(double z)
{
    if (z)
    {
        if (cosmoparams->OmegaK == 0)
        {
            return dist_comoving_LOS(z);
        }
        else if (cosmoparams->OmegaK > 0)
        {
            /* Calculate hubble distance */
            double dist_LOS = dist_comoving_LOS(z);
            double DH = C_CGS/cosmoparams->H0;
            double sinh_term = sinh(sqrt(cosmoparams->OmegaK) * dist_LOS / DH);
            return DH * sinh_term / sqrt(cosmoparams->OmegaK);
        }
        else if (cosmoparams->OmegaK < 0)
        {
            /* Calculate hubble distance */
            double dist_LOS = dist_comoving_LOS(z);
            double DH = C_CGS/cosmoparams->H0;
            double sin_term = sin(sqrt( abs(cosmoparams->OmegaK) ) * dist_LOS / DH);
            return DH * sin_term / sqrt( abs(cosmoparams->OmegaK) );
        }
        else
        {
            printf(" -- WARNING: UNKNOWN OmegaK = %.4e case. UNSURE HOW TO CALCULATE ! -- \n", cosmoparams->OmegaK);
        }
    }
    else
    {
        return 0;
    }
}

double Cosmology::dist_luminosity(double z)
{
    if (z)
    {
        return dist_comoving_trans(z) * (1. + z);
    }
    else
    {
        return 0;
    }
}

double Cosmology::dist_angular_diam(double z)
{
    if (z)
    {
        return dist_comoving_trans(z) / (1. + z);
    }
    else
    {
        return 0;
    }
}

double Cosmology::dist_modulus(double z)
{
    if (z)
    {
        double d_L = dist_luminosity(z);
        return  5*log10(d_L / (10*PC_CGS));
    }
    else
    {
        return 0;
    }
}

double Cosmology::hubble_param(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];

    return cosmoparams->H0*time_evo(z, fn_args);
}

double Cosmology::density(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];
    double time_evolution = time_evo(z, fn_args);

    return rho_crit*time_evolution*time_evolution;
}



double time_lookback_integrand(double z, double *args)
{
    return 1./((1.+z)*time_evo(z, args));
}

double time_lookback_integrand_LN(double ln_z, double *args)
{
    double z = exp(ln_z);
    double integrand = time_lookback_integrand(z, args);

    return z * integrand;
}

double Cosmology::time_lookback(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];

    return RombergIntegral(time_lookback_integrand, fn_args, 0., z, integral_Rmmax, integral_acc);
}

double Cosmology::time_age(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];

    /* Guard against ln(0.) */
    if (z <= 0)
    {
        z = LN_MIN;
    }
    double ln_z = log(z);
    double ln_zinfty = log(z_infty);

    return RombergIntegral(time_lookback_integrand_LN, fn_args, ln_z, ln_zinfty, integral_Rmmax, integral_acc);
}

double time_conformal_integrand(double z, double *args)
{
    return 1./(time_evo(z, args));
}

double time_conformal_integrand_LN(double ln_z, double *args)
{
    double z = exp(ln_z);
    double integrand = time_conformal_integrand(z, args);

    return z * integrand;
}

double Cosmology::time_conformal(double z)
{
    double args[6] = {cosmoparams->OmegaM, cosmoparams->OmegaR, cosmoparams->OmegaL, cosmoparams->OmegaK, cosmoparams->w0, cosmoparams->wa};
    double *fn_args = &args[0];

    /* Guard against ln(0.) */
    if (z <= 0)
    {
        z = LN_MIN;
    }
    double ln_z = log(z);
    double ln_zinfty = log(z_infty);

    return RombergIntegral(time_conformal_integrand_LN, fn_args, ln_z, ln_zinfty, integral_Rmmax, integral_acc);
}