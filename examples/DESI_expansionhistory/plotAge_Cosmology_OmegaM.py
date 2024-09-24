import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.collections import LineCollection


plt.style.use("dstyle")
_ = plt.figure()


# set integration table paths
tables_fPath = "OmegaM_study/timetables"
tables_OGfPath = "integration_tables"

# set path to each table
tablepathDESI_CMB = tables_OGfPath + "/table_DESI.txt"
tablepathDESI_CMB_Pantheon = tables_OGfPath + "/table_Pantheon.txt"
tablepathDESI_CMB_DESY5 = tables_OGfPath + "/table_DESY5.txt"
tablepathDESI_CMB_Union3 = tables_OGfPath + "/table_Union3.txt"
tablepathLCDM = tables_OGfPath + "/table_Concordance.txt"

tablepaths_all = [tablepathDESI_CMB, tablepathDESI_CMB_Pantheon,
                  tablepathDESI_CMB_DESY5, tablepathDESI_CMB_Union3]

# set base of table file name
table_fNameBaseDESI_CMB = "/timetable_DESI_CMB_EoS"
table_fNameBaseDESI_CMB_Pantheon = "/timetable_DESI_CMB_Pantheon_EoS"
table_fNameBaseDESI_CMB_DESY5 = "/timetable_DESI_CMB_DESY5_EoS"
table_fNameBaseDESI_CMB_Union3 = "/timetable_DESI_CMB_Union3_EoS"

table_fNameBases_all = [table_fNameBaseDESI_CMB, table_fNameBaseDESI_CMB_Pantheon, 
                        table_fNameBaseDESI_CMB_DESY5, table_fNameBaseDESI_CMB_Union3]

# set figure name
figfNameDESI_CMB = "ageinfo_DESI_CMB_OmegaM.pdf"
figfNameDESI_CMB_Pantheon = "ageinfo_DESI_CMB_Pantheon_OmegaM.pdf"
figfNameDESI_CMB_DESY5 = "ageinfo_DESI_CMB_DESY5_OmegaM.pdf"
figfNameDESI_CMB_Union3 = "ageinfo_DESI_CMB_Union3_OmegaM.pdf"

figfNames_all = [figfNameDESI_CMB, figfNameDESI_CMB_Pantheon,
                 figfNameDESI_CMB_DESY5, figfNameDESI_CMB_Union3]

# set legend labels
strOmegaM = r"$\Omega_{\rm{m}} = $"

strlabel_LCDM = r"$\Lambda$ CDM (" + strOmegaM + r"$0.3000$)"
strlabel_DESI_CMB = "DESI+CMB (" + strOmegaM + r"$0.3069$)" 
strlabel_DESI_CMB_Pantheon = "DESI+CMB+Pantheon (" + strOmegaM + r"$0.3085$)"
strlabel_DESI_CMB_DESY5 = "DESI+CMB+DESY5 (" + strOmegaM + r"$0.3160$)"
strlabel_DESI_CMB_Union3 = "DESI+CMB+Union3 (" + strOmegaM + r"$0.3230$)"

strlabels_all = [strlabel_DESI_CMB, strlabel_DESI_CMB_Pantheon,
                 strlabel_DESI_CMB_DESY5, strlabel_DESI_CMB_Union3]

# define Concordance and LCDM arrays 
# ASSUME SAME TIME PARAMETER WAS USED - REUSE SCALE FACTOR ARRAY
nskiprows=10
table_Concordance = np.loadtxt(tablepathLCDM, skiprows=nskiprows, delimiter = ',')
a_arr = table_Concordance[:,2]
time_lookH0_Concordance = table_Concordance[:,3] # in units of H0
scalefn_Concordance = interp1d(time_lookH0_Concordance, a_arr)


for i in range(4):
    ###
    # choose cosmology-specific info
    ###
    tablepath_cosmology = tablepaths_all[i]
    table_fNameBase_cosmology = table_fNameBases_all[i]
    figfName_cosmology = figfNames_all[i]
    strlabel_Cosmology = strlabels_all[i]

    # load in table & create scalefn 
    table_Cosmology = np.loadtxt(tablepath_cosmology, skiprows=nskiprows, delimiter = ',')
    time_lookH0_Cosmology = table_Cosmology[:,3] # in units of H0

    print("Starting plot for ", strlabel_Cosmology)
    # create fig info
    fig, (ax_look, ax_lookdiff) = plt.subplots(figsize=(14,7), nrows=1, ncols=2)

    # create OmegaMatter array
    OmegaM = np.arange(0.3, 0.33, step=0.005)
    lines_lookback = []
    lines_lookbackdiff = []
    for i, curr_OmegaM in enumerate(OmegaM):
        table_fName = table_fNameBase_cosmology + f"_OmegaM{i:.0f}.txt"
        table_fPath = tables_fPath + table_fName

        ###
        # focus on integration table
        ###
        # grab table info
        nskiprows=10
        table_cosmo = np.loadtxt(table_fPath, skiprows=nskiprows, delimiter = ',')
        a_arr = table_cosmo[:,2]
        time_lookH0 = table_cosmo[:,3] # in units of H0

        time_lookH0_inConcordance_mask = time_lookH0 > np.min(time_lookH0_Concordance)
        time_lookH0_inConcordance_mask &= time_lookH0 < np.max(time_lookH0_Concordance)
        time_lookH0_inConcordance = time_lookH0[time_lookH0_inConcordance_mask]

        # combine xy coords for plot
        lookback_elm = np.zeros((a_arr.size, 2))
        lookbackdiff_elm = np.zeros((time_lookH0_inConcordance.size , 2))

        lookback_elm[:,0] = time_lookH0
        lookback_elm[:,1] = a_arr
    
        lookbackdiff_elm[:,0] = time_lookH0_inConcordance
        lookbackdiff_elm[:,1] = a_arr[time_lookH0_inConcordance_mask] / scalefn_Concordance(time_lookH0_inConcordance)

        lines_lookback.append(lookback_elm)
        lines_lookbackdiff.append(lookbackdiff_elm)

    # combine to a LineCollection object
    linescollec_look = LineCollection(lines_lookback, array=OmegaM, cmap='rainbow')
    linescollec_lookdiff = LineCollection(lines_lookbackdiff, array=OmegaM, cmap='rainbow')

    # add line collection to plot
    _ = ax_look.add_collection(linescollec_look)
    _ = ax_lookdiff.add_collection(linescollec_lookdiff)

    # add concordance and cosmology to plot
    _ = ax_look.plot(time_lookH0_Concordance, a_arr, label=strlabel_LCDM, ls='--')
    _ = ax_look.plot(time_lookH0_Cosmology, a_arr, label=strlabel_Cosmology, ls='--')

    _ = ax_lookdiff.plot(time_lookH0_Concordance[1:], a_arr[1:] / scalefn_Concordance(time_lookH0_Concordance[1:]), 
                        label=strlabel_LCDM, ls='--')
    _ = ax_lookdiff.plot(time_lookH0_Cosmology[1:], a_arr[1:] / scalefn_Concordance(time_lookH0_Cosmology[1:]), 
                        label=strlabel_Cosmology, ls='--')


    # add colorbar
    divider_lookdiff = make_axes_locatable(ax_lookdiff)
    cbar_lookdiff = divider_lookdiff.append_axes("right", size="6%", pad=0.4)
    _ = fig.add_axes(cbar_lookdiff)
    _ = fig.colorbar(linescollec_lookdiff, cax=cbar_lookdiff, orientation="vertical")
    _ = cbar_lookdiff.yaxis.set_ticks_position('right')

    cbar_str = r"$\Omega_{\rm{m}}$"
    _ = cbar_lookdiff.set_ylabel(cbar_str, rotation=270)
    cbar_lookdiff.yaxis.labelpad = 20


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
    _ = fig.savefig(figfName_cosmology, dpi=256)
    
    plt.close(fig)

    print("Completed plot for ", strlabel_Cosmology)

