# FlatCosmoCalc


## Cosmology Details

Currently, this calculator is used to study dark energy equation of state, so the implementation calculates $\Omega_{\Lambda} = 1 - \Omega_{\gamma} - \Omega_{\textrm{m}} - \Omega_{K}$.

To create an integration table, two parameter files are required:

### CosmoParams

Present-day cosmological parameters:

```
H0=                     # present day Hubble Parameter in (km/s/Mpc)
OmegaM=                 # present day Matter energy density
OmegaK=                 # present day Curvature energy density
OmegaR=                 # present day Radiation energy density
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
log=(Y/N)                # whether to run on log(1+z)
zmin=                    # lower redshift (present-day)
zmax=                    # upper redshift (earlier in Universe)
ntime=                   # number of integration points
```


### Cosmology Equations

The equations implemented are saved in `cosmology_functions.cpp`.

\textbf{HAVE TO WRITE EQUATIONS HERE AND INCLUDE REFERENCES IN CODE}

To specify which equations to integrate and save, we use compiler flags, with examples in `builds`.

All equations are computed in cgs units.


## Running Calculator

First need to make the binary using the MakeFile

```
make TYPE=[]
```

which should create a binary file `flatcosmocalc.[type].[build]` in `bin` directory.

To run the calculator just run

```
./flatcosmocalc.[type].[build] CosmoParams.txt TimeParams.txt IntTable.txt
```

where `CosmoParams.txt` holds the cosmological parameters, `TimeParams.txt` holds the time information, and `IntTable.txt` is the outgoing file that will hold the information for the integration table.

## Tests

In the `test` subdirectory, there are two jupyter notebooks that serve as benchmarks that things are working well.

`AstropyTest.ipynb` - Compares table computed with Planck parameters against astropy

`IntegratorTest.ipynb` - Compares some integration tables against numpy


## Integration

To create this calculator, we coded up an implementation of Romberg's integration method. To ensure that this method is accurate, we created a `integral_test` file in the `integrator` subdirectory. This file has a struct definition and six  functions in testing my integrator in `integrator.cpp`:

`
struct IntegratorTestInfo
{
    float a;            # lower limit of integration
    float b;            # upper limit of integration
    int nmax;           # maximum n-th order of integration
    int nints;          # number of integral performances between a and b
    float acc;          # accuracy to end integration
    char outFile[MAXLEN];# where to place outgoing integration table
};
`

1. `Parse_IntegralParams()` - populate an instance of `IntegratorTestInfo`
2. `Parse_IntegralParam()` - populate one parameter of `IntegratorTestInfo`
3. `CreateIntegralTable()` - allocate memory to integral table
4. `DestroyIntegralTable()` - de-allocate / free memory of integral table
5. `SaveIntegralTable()` - save the integral table
6. `RunTest()` - which performs the previous functions given just an integral param file

A produced integral table will output

$$
\int_a^x f(x') \textrm{d}x' = F(x) - F(a)
$$

where $x \in (a,b)$, and saves $x$ and $F(x) - F(a)$ (computed integral value).

There are example integrand $f(x)$ in the `integrands.cpp` that are called if the `TEST_INTEGRATOR` flag is passed through during compilation. By default, the integrator will run on the integral

$$
\int_a^x \frac{4}{1 + y^2} \textrm{d}y = 4 (\arctan(x) - \arctan(a))
$$

such that passing in $a=0$ and $b=1$ gives an approximation of $\pi$.

The `RunTest()` expects an integral test parameter file that has the following information:

```
a=// lower limit of integration
b=// upper limit of integration
nmax=// upper limit of n-order Romberg integraion
acc=// accuracy at which to stop integrating
nints=// number of integrals between a and b
outFile=// destination of integration table
```
