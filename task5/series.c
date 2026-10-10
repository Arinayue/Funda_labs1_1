#include "series.h"
#include <math.h>
#include <stddef.h>

#define MAX_SERIES_ITERATIONS 10000000

status sum_a(const double x, const double eps, double *out_val) {
    if (out_val == NULL || eps <= 0.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double sum = 1.0;     
    double fact_n = 1.0;  
    double pow_x = 1.0;   
    double term = 1.0;    
    int n = 0;

    while (fabs(term) >= eps) {
        n++;

        fact_n *= (double)n;

        pow_x *= x;

        term = pow_x / fact_n;

        sum += term;

        if (n >= MAX_SERIES_ITERATIONS) {
            return ERROR_CONVERGENCE_FAILED;
        }
    }

    *out_val = sum;
    return SUCCESS;
}

status sum_b(const double x, const double eps, double *out_val) {
    if (out_val == NULL || eps <= 0.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double sum = 1.0;      
    double fact_2n = 1.0; 
    double pow_x = 1.0;   
    double term = 1.0;
    int n = 0;

    while (fabs(term) >= eps) {
        n++;

        fact_2n *= (2.0 * n - 1.0) * (2.0 * n);

        pow_x *= (x * x);

        double sign = (n % 2 == 0) ? 1.0 : -1.0;
        term = sign * (pow_x / fact_2n);

        sum += term;

        if (n >= MAX_SERIES_ITERATIONS) {
            return ERROR_CONVERGENCE_FAILED;
        }
    }

    *out_val = sum;
    return SUCCESS;
}

status sum_c(const double x, const double eps, double *out_val) {
    if (out_val == NULL || eps <= 0.0 || fabs(x) >= 1.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double sum = 1.0;          
    double fact_n_cube = 1.0;  
    double fact_3n = 1.0;      
    double pow_27 = 1.0;       
    double pow_x = 1.0;       
    double term = 1.0;
    int n = 0;

    while (fabs(term) >= eps) {
        n++;

        fact_n_cube *= ((double)n * n * n);

        fact_3n *= (3.0 * n - 2.0) * (3.0 * n - 1.0) * (3.0 * n);

        pow_27 *= 27.0;

        pow_x *= (x * x);

       
        term = (pow_27 * fact_n_cube * pow_x) / fact_3n;

        sum += term;

        if (n >= MAX_SERIES_ITERATIONS) {
            return ERROR_CONVERGENCE_FAILED;
        }
    }

    *out_val = sum;
    return SUCCESS;
}


status sum_d(const double x, const double eps, double *out_val) {
    if (out_val == NULL || eps <= 0.0 || fabs(x) >= 1.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double dfact_num = 1.0; 
    double dfact_den = 2.0; 
    double pow_x = x * x;   
    double term = - (dfact_num / dfact_den) * pow_x; 
    double sum = term;
    int n = 1;

    while (fabs(term) >= eps) {
        n++;

        dfact_num *= (2.0 * n - 1.0);

        dfact_den *= (2.0 * n);

        pow_x *= (x * x);

        double sign = (n % 2 == 0) ? 1.0 : -1.0;
        term = sign * (dfact_num / dfact_den) * pow_x;

        sum += term;

        if (n >= MAX_SERIES_ITERATIONS) {
            return ERROR_CONVERGENCE_FAILED;
        }
    }

    *out_val = sum;
    return SUCCESS;
}