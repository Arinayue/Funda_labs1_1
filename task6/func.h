#ifndef FUNC_H
#define FUNC_H

#include <stddef.h>

#include "status.h"

status is_convex_polygon(int *result, double eps, int n, ...);

status poly_value(double *result, double x, int n, ...);

status find_kaprekar(const char **found, size_t *found_count, int base, int count, ...);

status geometric_mean(double *result, double eps, int count, ...);

status fast_pow(double *result, double base, int exp, double eps);

status bisect_root(double *root, double a, double b, double eps, double (*f)(double));

#endif