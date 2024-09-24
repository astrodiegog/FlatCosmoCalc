#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integrator/integrator.h"

#ifdef TEST_INTEGRATOR
    #include "integrator/integrands.h"
    #include "integrator/integral_test.h"
#else
    #include "cosmology/cosmology.h"
    #include "cosmology/cosmoparams.h"
#endif

int main(int argc, char **argv)
{

#ifdef HOWDY
    printf(" --- HOWDY! --- \n");
#endif

#ifdef TEST_INTEGRATOR

    if (argc < 1)
    {
        fprintf(stderr, "No Integral Param directory ! ahh \n");
    }

    RunAllTests();

#else
    /* Declare cosmo & time parameter file */
    char *cosmoparam_file, *timeparam_file;

    /* Declare fname for integration table */
    char *table_outfname;

    /* Declare total energy-density */
    double Omega_tot;

    /* Declare CosmoParams & TimeParams structs */
    struct CosmoParams cosmoparams;
    struct TimeDomainParams timedomainparams;

    if (argc < 4)
    {
        fprintf(stderr, "No Outgoing Table file name! ahh \n");
        return 0;
    }
    else
    {
        cosmoparam_file = argv[1];
        timeparam_file = argv[2];
        table_outfname = argv[3];
    }

    /* Create & populate cosmoparams */
    Parse_CosmoParams(cosmoparam_file, &cosmoparams);

    /* Test flatness */
    Omega_tot = cosmoparams.OmegaM + cosmoparams.OmegaK + cosmoparams.OmegaR + cosmoparams.OmegaL;
    if( Omega_tot != 1.0 )
    {
        printf("ERROR: The provided energy-densities to do not correspond \n");
        printf("\t to a present-day flat Universe. Please change \n");
        printf("\t the input cosmological parameters. ty! (: \n");

        return 0;
    }

    /* Create & populate timedomainparams */
    Parse_TimeParams(timeparam_file, &timedomainparams);

    /* Create Cosmology object with param structs */
    Cosmology cosmology(&cosmoparams, &timedomainparams);

    printf("Created a cosmology object! \n");
    printf("Let's print out the cosmo params & time info\n");
    cosmology.PrintCosmoParams();
    cosmology.PrintTimeDomainParams();
    
    double **integral_table = cosmology.GetIntegrationTable();

    cosmology.SaveIntegrationTable(integral_table, table_outfname);

    cosmology.DestroyIntegrationTable(integral_table);

#endif

    return 0;
}
