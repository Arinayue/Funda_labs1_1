#include "integration.h"
#include <math.h>
#include <stddef.h>

status func_a(const double x, const double eps, double *out_val) {
    if (out_val == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (fabs(x) < eps) {
        *out_val = 1.0;
    } else {
        *out_val = log(1.0 + x) / x;
    }
    return SUCCESS;
}

status func_b(const double x, const double eps, double *out_val) {
    (void)eps;
    if (out_val == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    *out_val = exp(-x * x / 2.0);
    return SUCCESS;
}

status func_c(const double x, const double eps, double *out_val) {
    (void)eps;
    if (out_val == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (1.0 - x <= 0.0) {
        return ERROR_INVALID_ARGUMENTS;
    }
    *out_val = log(1.0 / (1.0 - x));
    return SUCCESS;
}

status func_d(const double x, const double eps, double *out_val) {
    if (out_val == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (fabs(x) < eps) {
        *out_val = 1.0;
    } else {
        *out_val = pow(x, x);
    }
    return SUCCESS;
}

status integrate(const integrand_t f, const double a, const double b, const double eps, double *out_val) {
    if (f == NULL || out_val == NULL || eps <= 0.0 || a >= b) {
        return ERROR_INVALID_ARGUMENTS;
    }

    const int n = 1000; 
    const double h = (b - a) / n;
    double sum = 0.0;
    double fx;
    status err;

    if ((err = f(a, eps, &fx)) != SUCCESS) return err;
    sum += fx;

    if ((err = f(b, eps, &fx)) != SUCCESS) return err;
    sum += fx;

    for (int i = 1; i < n; i++) {
        const double x = a + i * h;
        if ((err = f(x, eps, &fx)) != SUCCESS) return err;

        if (i % 2 == 1) {
            sum += 4.0 * fx;
        } else {
            sum += 2.0 * fx;
        }
    }

    *out_val = sum * (h / 3.0);
    return SUCCESS;
}