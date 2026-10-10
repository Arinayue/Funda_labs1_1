#ifndef FUNC_H
#define FUNC_H

#include "status.h"

typedef struct {
    double a, b, c; 
    double x1;     
    double x2;     
    int count;     
} Solution;

status solve_single_q(const double eps, const double a, const double b, const double c, Solution *out_sol);

status func_q(const double eps, const double a, const double b, const double c, Solution *out_solutions, int *out_unique_count);

status func_m(const long a, const long b, int *out_result);

status func_t(const double eps, const double a, const double b, const double c, int *out_result);

#endif