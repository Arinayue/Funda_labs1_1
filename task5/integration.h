#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "status.h"

typedef status (*integrand_t)(const double x, const double eps, double *out_val);

status func_a(const double x, const double eps, double *out_val);
status func_b(const double x, const double eps, double *out_val);
status func_c(const double x, const double eps, double *out_val);
status func_d(const double x, const double eps, double *out_val);

status integrate(const integrand_t f, const double a, const double b, const double eps, double *out_val);

#endif