#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "status.h"

typedef status_code (*func_math)(const double eps, double* out_val);

typedef struct {
    const char* const_name;
    const char* method_name;
    func_math func;
} ConstantMethod;

unsigned long long fact(const unsigned long long n);

status_code e_limit(const double eps, double* out_val);
status_code e_series(const double eps, double* out_val);
status_code e_equation(const double eps, double* out_val);

status_code pi_limit(const double eps, double* out_val);
status_code pi_series(const double eps, double* out_val);
status_code pi_equation(const double eps, double* out_val);

status_code ln2_limit(const double eps, double* out_val);
status_code ln2_series(const double eps, double* out_val);
status_code ln2_equation(const double eps, double* out_val);

status_code sqrt2_limit(const double eps, double* out_val);
status_code sqrt2_series(const double eps, double* out_val);
status_code sqrt2_equation(const double eps, double* out_val);

status_code gamma_limit(const double eps, double* out_val);
status_code gamma_series(const double eps, double* out_val);
status_code gamma_equation(const double eps, double* out_val);

#endif