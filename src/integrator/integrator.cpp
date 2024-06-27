#include <stdio.h>

double my_fabs(double num)
{
    if (num > 0)
    {
        return num;
    }
    else
    {
        return -num;
    }
}

double my_pow(double base, int exponent)
{
    /* Declare iterator */
    int i;

    /* Declare the answer to return */
    double answer = 1.;

    for (i = 0; i < exponent; i++)
    {
        answer *= base;
    }

    return answer;
}


double R_m0_sum(double (*f)(double, double*), double *fn_args, double a, double b, int n)
{
    /* Declare for loop iterator */
    int i;

    /* Declare sum and integration variables */
    double summa, h_n, x;

    /* Calculate the step width size */
    h_n = (b - a) / my_pow(2, n);

    /* Initialize for loop sum for this process */
    summa = 0.0;
    
    for (i = 1; i <= my_pow(2, n - 1); i ++)
    {
        x = a + (2 * (double)i - 1) * h_n;

        summa += f(x, fn_args);
    }

    return summa * h_n;
}



double RombergIntegral(double (*f)(double, double*), double *fn_args, double a, double b, int n_max, double acc)
{
    /* Declare the iterator for which n-step to use */
    int n;

    /* Declare iterator to fill out R(-,j) array */
    int m;

    /* Declare R(n,0) sum value*/
    double summa;

    /* Declare number to calculate 4^m */
    double fourM;

    /* Declare Romberg R(i-1,j) and R(i,j) arrays and pointers*/
    double R_old[n_max], R_curr[n_max];
    double *pR_old = &R_old[0];
    double *pR_curr = &R_curr[0];
    double *pR_temp;

    /* Calculate R(0,0) */
    pR_old[0] = (0.5) * (b - a) * (f(a, fn_args) + f(b, fn_args));

    if (n_max == 0)
    {
        return pR_old[0];
    }

    for (n = 1; n <= n_max; n++)
    {
        /* Calculate sum for R(n,0) */
        summa = R_m0_sum(f, fn_args, a, b, n);

        /* Place R(n,0) */
        pR_curr[0] = summa + (0.5 * pR_old[0]);

        /* Fill out current row R(n,m) */
        for (m = 1; m <= n; m++)
        {
            fourM = my_pow(4, m);
            pR_curr[m] = (1 / (fourM - 1)) * (fourM * pR_curr[m - 1] - pR_old[m - 1]);
        }

        /* Print out current row if DEBUG */
    #ifdef DEBUG_INTEGRATOR
        printf("Row %d: \t", n);
        for (m = 0; m <= n; m++)
        {
            printf("%.8f \t", pR_curr[m]);
        }
        printf("\n");
    #endif

        /* Check accuracy condition after first row */
        if ((n > 1) && ((my_fabs(pR_old[n - 1] - pR_curr[n])) < acc))
        {
            /* Break out of for loop */
        #ifdef DEBUG_INTEGRATOR
            printf("Error is: %.4e\n", (my_fabs(pR_old[n - 1] - pR_curr[n])));
        #endif
            break;
        }
        else
        {
            pR_temp = pR_old;
            pR_old = pR_curr;
            pR_curr = pR_temp;
        }
    }

    return pR_curr[n];
}