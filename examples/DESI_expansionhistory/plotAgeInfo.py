import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d


plt.style.use("dstyle")
_ = plt.figure()


# set integration table paths
tables_fPath = "integration_tables"
tablepathLCDM = tables_fPath + "/table_Concordance.txt"
tablepathDESI_CMB = tables_fPath + "/table_DESI.txt"
tablepathDESI_CMB_Pantheon = tables_fPath + "/table_Pantheon.txt"
tablepathDESI_CMB_DESY5 = tables_fPath + "/table_DESY5.txt"
tablepathDESI_CMB_Union3 = tables_fPath + "/table_Union3.txt"
tablepath_all = [tablepathLCDM, tablepathDESI_CMB, tablepathDESI_CMB_Pantheon,
                 tablepathDESI_CMB_DESY5, tablepathDESI_CMB_Union3]

# set legend labels
strlabel_LCDM = r"$\Lambda$ CDM"
strlabel_CMB = "DESI+CMB"
strlabel_CMB_Pantheon = "DESI+CMB+Pantheon"
strlabel_CMB_DESY5 = "DESI+CMB+DESY5"
strlabel_CMB_Union3 = "DESI+CMB+Union3"
strlabel_all = [strlabel_LCDM, strlabel_CMB, strlabel_CMB_Pantheon, 
                strlabel_CMB_DESY5, strlabel_CMB_Union3]

# create fig info + fname
fig, (ax_look, ax_lookdiff) = plt.subplots(figsize=(14,7), nrows=1, ncols=2)
figfName = "ageinfo.pdf"

for i, tablePath in enumerate(tablepath_all):
    strlabel = strlabel_all[i]

    ###
    # focus on integration table
    ###
    # grab table info
    nskiprows=10
    table_cosmo = np.loadtxt(tablePath, skiprows=nskiprows, delimiter = ',')
    z_arr = table_cosmo[:,0]
    a_arr = table_cosmo[:,2]
    time_lookH0 = table_cosmo[:,3] # in units of H0

    if (i == 0):
        # need to define Concordance Scale Factor function
        scalefn_Concordance = interp1d(time_lookH0, a_arr)

    # plot info
    ax_look.plot(time_lookH0, a_arr, label=strlabel)
    ax_lookdiff.plot(time_lookH0[1:], a_arr[1:] / scalefn_Concordance(time_lookH0[1:]), label=strlabel)

    # set labels
    xlabel0 = r"Lookback time ($H_0 t$)"
    ylabel0 = r"Scale factor"
    ylabel1 = r"Scale factor ($a / a_{\Lambda}$)"
    
    ax_look.set_xlabel(xlabel0), ax_look.set_ylabel(ylabel0)
    ax_lookdiff.set_xlabel(xlabel0), ax_lookdiff.set_ylabel(ylabel1)

    # set limits
    xlow0, xupp0 = 1., 0.
    xlow1, xupp1 = 0.8, 0.
    ylow0, yupp0 = 0., 1.
    ylow1, yupp1 = 0.89, 1.01

    ax_look.set_xlim(xlow0, xupp0), ax_lookdiff.set_xlim(xlow1, xupp1)
    ax_look.set_ylim(ylow0, yupp0), ax_lookdiff.set_ylim(ylow1, yupp1)

    # add background grid
    _ = ax_look.grid(which='both', axis='both', alpha=0.3)
    _ = ax_lookdiff.grid(which='both', axis='both', alpha=0.3)

    # add legend
    _ = ax_look.legend(fontsize=15, loc='upper left')
    _ = ax_lookdiff.legend(fontsize=15, loc='lower right')


_ = fig.tight_layout()
_ = fig.savefig(figfName, dpi=256)


