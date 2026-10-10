#ifndef SERIES_H
#define SERIES_H

#include "status.h"

status sum_a(const double x, const double eps, double *out_val);
status sum_b(const double x, const double eps, double *out_val);
status sum_c(const double x, const double eps, double *out_val);
status sum_d(const double x, const double eps, double *out_val);

#endif