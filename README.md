# FlatCosmoCalc


## Cosmology Details

To create an integration table, two parameter files are required:

### CosmoParams

Present-day cosmological parameters:

```
H0=                     # present day Hubble Parameter in (km/s/Mpc)
OmegaM=                 # present day Matter energy density
OmegaK=                 # present day Curvature energy density
OmegaR=                 # present day Radiation energy density
OmegaL=                 # present day Vacuum energy density
w0=                     # constant term for dark energy equation of state
wa=                     # linear term for dark energy equation of state
```

The dark energy equation of state takes the form

$$
w(a) = w_0 + w_{\textrm{a}} ( 1 - a )
$$


### TimeParams

Redshift domain for the integration table:

```
log=(Y/N)                # whether to run on log(1+z) or lin(z)
zmin=                    # lower redshift (present-day)
zmax=                    # upper redshift (earlier in Universe)
ntime=                   # number of integration points
```


### Cosmology Equations

The equations implemented are saved in `cosmology_functions.cpp`. A helpful review of the quantities can be found in David Hogg's [Distance measures in cosmology](https://ui.adsabs.harvard.edu/abs/1999astro.ph..5116H/abstract).

First we define the time evolution function

$$
\xi(z) = \sqrt{\Omega_{k}(1 + z)^{2} + \Omega_{\textrm{m}}(1 + z)^{3} + \Omega_{\textrm{R}}(1 + z)^{4} + \Omega_{\Lambda} (1+z)^{3(1 + w_0 + w_a)} \exp \left[ \frac{-3 w_a z}{1 + z} \right] }
$$

where the $\Omega$ values correspond to

- $\Omega_k$ - present-day energy-density related to the spatial curvature of the Universe
- $\Omega_{\textrm{m}}$ - present-day energy-density related to the matter of the Universe (including both baryons and dark matter)
- $\Omega_{\textrm{R}}$ - present-day energy-density related to the relativistic species of the Universe (including photons and neutrinos)
- $\Omega_{\Lambda}$ - present-day energy-density related to the vacuum density of the Universe

Age of the Universe at some redshift

$$
t_{\textrm{age}}(z) H_0  = \int_{z}^{\infty} \frac{1}{(1 + z') \xi(z')} \textrm{d}z'
$$

Lookback time to some redshift

$$
t_{\textrm{look}}(z) H_0  = \int_{0}^{z} \frac{1}{(1 + z') \xi(z')} \textrm{d}z'
$$

Both quantities are also computed in conformal time

$$
\chi_{\textrm{age}}(z) H_0  = \int_{z}^{\infty} \frac{1}{ \xi(z')} \textrm{d}z'
$$

$$
\chi_{\textrm{look}}(z) H_0  = \int_{0}^{z} \frac{1}{\xi(z')} \textrm{d}z'
$$

Line-of-sight comoving distance

$$
d_{\textrm{LOS}}(z) = D_H \int_0^z \frac{1}{\xi(z')} \textrm{d}z'
$$

where we adopt the notation from David Hogg that $D_H$ is the hubble distance calculated as $D_H = c / H_0$. All distances in the calculator are saved in units of Megaparsecs.

Transverse comoving distance

$$
d_{\textrm{trans}}(z) = \begin{cases}
D_H \sin \left[ \sqrt{|\Omega_{k}| \left(d_{\textrm{LOS}}(z) / D_{H} \right) } \right] / \sqrt{|\Omega_{k}|} & \mathrm{if} & \Omega_{k} < 0 \\
D_H \sinh \left[ \sqrt{\Omega_{k} \left(d_{\textrm{LOS}}(z) / D_{H} \right) } \right] / \sqrt{\Omega_{k}} & \mathrm{if} & \Omega_{k} > 0 \\
d_{\textrm{LOS}}(z) & \mathrm{if} & \Omega_{k} = 0 \\
\end{cases}
$$

Luminosity distance

$$
d_{\textrm{lum}} = d_{\textrm{trans}}(z) (1 + z)
$$

Angular diameter distance

$$
d_{\textrm{ang}} = d_{\textrm{trans}}(z) / (1 + z)
$$

Distance modulus

$$
\mu(z) = 5 \log_{10} \left(\frac{d_{\textrm{lum}}}{ 10 \textrm{pc}} \right)
$$

Hubble parameter

$$
H(z) = H_0 \xi(z)
$$

Density parameter

$$
\rho(z) = \rho_c \xi(z)^2
$$

where $\rho_c$ is the critical density of the Universe

$$
\rho_c = \frac{3 H_0^2}{8 \pi G}
$$

where $G$ is the gravitational constant.

To specify which equations to integrate and save, we use compiler flags, with examples in `builds`.

All equations are computed in the backend using cgs units.


## Running Calculator

First we need to make the binary using the MakeFile

```
make TYPE=[]
```

which should create a binary file `flatcosmocalc.[type].[build]` in `bin` directory.

To run the calculator, run

```
./flatcosmocalc.[type].[build] CosmoParams.txt TimeParams.txt IntTable.txt
```

where `CosmoParams.txt` holds the cosmological parameters, `TimeParams.txt` holds the time information, and `IntTable.txt` is the outgoing file that will hold the information for the integration table.

## Tests

In the `test` subdirectory, there are two jupyter notebooks that serve as benchmarks that things are working well.

`AstropyTest.ipynb` - Compares table computed with Planck2018 parameters against astropy

`IntegratorTest.ipynb` - Compares some integration tables against analytical solutions


## Integration

To create this calculator, we used an implementation of Romberg's integration method. To ensure that this method is accurate, we created a `integral_test` file in the `integrator` subdirectory. This file has a struct definition and six functions in testing the integrator in `integrator.cpp`:

```
struct IntegratorTestInfo
{
    float a;            # lower limit of integration 
    float b;            # upper limit of integration
    int nmax;           # maximum n-th order of integration
    int nints;          # number of integral performances between a and b
    float acc;          # accuracy to end integration
    bool log;           # whether domain is in log-space
    char outFile[MAXLEN];# where to place outgoing integration table
};
```

1. `Parse_IntegralParams()` - populate an instance of `IntegratorTestInfo`
2. `Parse_IntegralParam()` - populate one parameter of `IntegratorTestInfo`
3. `CreateIntegralTable()` - allocate memory to integral table & perform integral
4. `CreateIntegralTableLN()` - allocate memory to integral table & perform integral in log space
5. `DestroyIntegralTable()` - de-allocate / free memory of integral table
6. `SaveIntegralTable()` - save the integral table
7. `RunTest()` - which performs the previous functions given just an integral param file

A produced integral table will save the integral

$$
\int_a^x f(x') \textrm{d}x'
$$

as well as the $x$ value $x \in (a,b)$.

The `RunTest()` routine expects an integral test parameter file that has the following information:

```
a=// lower limit of integration
b=// upper limit of integration
nmax=// upper limit of n-order Romberg integraion
acc=// accuracy at which to stop integrating
nints=// number of integrals between a and b
log=// whether to run on a logspace domain
outFile=// destination of integration table
```

The reason for the logspace domain is because we commonly are integrating over large orders of magnitude, and a linearly spaced domain may not always capture the desired accuracy that we are interested in. To do this, we complete a change of variables with $u = \ln(x)$ such that $\textrm{d}u = \textrm{d}x / x$ where we can change our integral to instead be

$$
\int_{a}^{x} f(x') \textrm{d}x' \rightarrow \int_{\ln(a)}^{\ln(x)} x' f(x') \textrm{d}\left( \ln(x') \right) = \int_{\ln(a)}^{\ln(x)} f(e^u) e^u \textrm{d}u
$$

If the logspace domain flag is selected in the integral test parameter file, the integral table will instead save $\ln(x)$. Routines in `integrands.cpp` that are suffixed with `_LN` are functions that expect $\ln(x)$ and return the function $x f(x)$.
