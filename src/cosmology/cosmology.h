#define MPC_CGS     3.0857e24        // Mpc in cm
#define PC_CGS      3.0857e18        // Mpc in cm
#define KM_CGS      1e5              // km in cm
#define C_CGS       2.99792458e10    // speed of light in cm/s
#define G_CGS       6.67259e-8       // gravitational constant, cgs
#define PI          3.141592653      // pi!

/* Holds global info at present z=0 day and routines to go back in time */
class Cosmology
{
    struct CosmoParams *cosmoparams;
    struct TimeDomainParams *timedomainparams;

public:
    /* Critical density present-day */
    double rho_crit;

    /* Integrator details */
    double z_infty, integral_acc;
    int integral_Rmmax;

    /* Integration table details */
    int ndata_table;

    /* Declare constructor method */
    Cosmology(struct CosmoParams *cosmoparams, struct TimeDomainParams *timedomainparams);

    /* Get a dynamically-allocated time domain array */
    double *GetTimeArray();

    /* Return lookback time to a redshift */
    double time_lookback(double z);

    /* Return Universe age at a redshift */
    double time_age(double z);

    /* Return conformal time to a redshift */
    double time_conformal(double z);

    /* Return comoving line of sight distance to some redshift */
    double dist_comoving_LOS(double z);

    /* Return comoving transverse distance to some redshift */
    double dist_comoving_trans(double z);

    /* Return luminosity distance to some redshift */
    double dist_luminosity(double z);

    /* Return angular diameter distance to some redshift */
    double dist_angular_diam(double z);

    /* Return distance modulus to some redshift */
    double dist_modulus(double z);

    /* Return Hubble parameter at some redshift */
    double hubble_param(double z);

    /* Return density normalized by present critical density at some redshift */
    double density(double z);

    /* Print Cosmo Params */
    void PrintCosmoParams();

    /* Print Time Domain Params */
    void PrintTimeDomainParams();

    /* Return integration table */
    double **GetIntegrationTable();

    /* Save integration table */
    void SaveIntegrationTable(double **integral_table, char *fname);

    /* Free integration table */
    void DestroyIntegrationTable(double **integral_table);
};

/* Define info needed to the cosmology at a snapshot */
struct CosmoSnapshot
{
    double a_snap;
    double z_snap;
    double l_z_snap;

#ifdef SAVE_TIME_LOOKBACK
    double time_lookback;
#endif

#ifdef SAVE_TIME_AGE
    double time_age;
#endif

#ifdef SAVE_TIME_CONFORMAL
    double time_conformal;
#endif

#ifdef SAVE_DIST_LUM
    double dist_lum;
#endif

#ifdef SAVE_DIST_COMOVING_LOS
    double dist_comoving_los;
#endif

#ifdef SAVE_DIST_COMOVING_TRANS
    double dist_comoving_trans;
#endif

#ifdef SAVE_DIST_ANG_DIAM
    double dist_angdiam;
#endif

#ifdef SAVE_DIST_MOD
    double dist_mod;
#endif

#ifdef SAVE_HUBBLE
    double hubble;
#endif

#ifdef SAVE_DENSITY
    double density;
#endif

};

