/*\fn void Parse_CosmoParams(char *, struct* CosmoParams) */
/* Routine to place info from cosmo param file onto CosmoParams struct */
extern void Parse_CosmoParams(char *cosmoparam_file, struct CosmoParams *cosmoparams);

/*\fn void Parse_CosmoParam(char *, char *, struct* CosmoParams) */
/* Routine to place key-value pair onto CosmoParams struct */
extern void Parse_CosmoParam(char *key, char *value, struct CosmoParams *cosmoparams);

/*\fn void Parse_TimeParams(char *, struct* TimeDomainParams) */
/* Routine to place info from time param file onto TimeDomainParams struct */
extern void Parse_TimeParams(char *timedomainparam_file, struct TimeDomainParams *timedomain);

/*\fn void Parse_TimeParam(char *, char *, struct* TimeDomainParams) */
/* Routine to place key-value pair onto TimeDomainParams struct */
extern void Parse_TimeParam(char *key, char *value, struct TimeDomainParams *timedomain);

/* Define max string buffer lengths */
#define MAXLEN 1028

struct CosmoParams
{
    /* Declare default present hubble param and densities */
    double H0, OmegaM, OmegaK, OmegaR, OmegaL;

    /* Declare linearly evolving equation of state for Dark Energy */
    double w0, wa;
};

struct TimeDomainParams
{
    /* Declare beginning and ending times for integration table */
    double zmin, zmax;
    double amin, amax;

    /* Declare whether to make a logspace array */
    bool log;

    /* Declare how many data points to use */
    int ntime;
};

