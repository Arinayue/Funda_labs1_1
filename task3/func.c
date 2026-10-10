#include "func.h"
#include <stdlib.h>
#include <math.h>

status solve_single_q(const double eps, const double a, const double b, const double c, Solution *out_sol) {
    if (out_sol == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (eps < 0.0 || eps > 1.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    out_sol->a = a;
    out_sol->b = b;
    out_sol->c = c;
    out_sol->x1 = 0.0;
    out_sol->x2 = 0.0;

    if (fabs(a) <= eps) {
        if (fabs(b) <= eps) {
            out_sol->count = (fabs(c) <= eps) ? -1 : 0;
        } else {
            out_sol->x1 = -c / b;
            out_sol->count = 1;
        }
        return SUCCESS;
    }

    double disc = b * b - 4.0 * a * c;

    if (disc < -eps) {
        out_sol->count = 0;
    } else if (fabs(disc) <= eps) {
        out_sol->x1 = -b / (2.0 * a);
        out_sol->count = 1;
    } else {
        double sqrt_d = sqrt(disc);
        out_sol->x1 = (-b - sqrt_d) / (2.0 * a);
        out_sol->x2 = (-b + sqrt_d) / (2.0 * a);
        out_sol->count = 2;
    }

    return SUCCESS;
}

status func_m(const long a, const long b, int *out_result) {
    if (out_result == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (b == 0) {
        return ERROR_ZERO_DIVISION;
    }

    if (a % b == 0) {
        *out_result = 1;
    } else {
        *out_result = 0;
    }
    return SUCCESS;
}

status func_t(const double eps, const double a, const double b, const double c, int *out_result) {
    if (eps < 0.0 || eps > 1.0 || a <= 0.0 || b <= 0.0 || c <= 0.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (out_result == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double a2 = a * a;
    double b2 = b * b;
    double c2 = c * c;

    if (fabs(a2 + b2 - c2) <= eps || fabs(a2 + c2 - b2) <= eps || fabs(b2 + c2 - a2) <= eps) {
        *out_result = 1; 
    } else {
        *out_result = 0; 
    }

    return SUCCESS;
}

status func_q(const double eps, const double a, const double b, const double c, Solution *out_solutions, int *out_unique_count) {
    if (out_solutions == NULL || out_unique_count == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (eps < 0.0 || eps > 1.0) {
        return ERROR_INVALID_ARGUMENTS;
    }

    double perms[6][3] = {
        {a, b, c}, {a, c, b},
        {b, a, c}, {b, c, a},
        {c, a, b}, {c, b, a}
    };

    int unique_cnt = 0;

    for (int i = 0; i < 6; i++) {
        double cur_a = perms[i][0];
        double cur_b = perms[i][1];
        double cur_c = perms[i][2];

        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (fabs(cur_a - perms[j][0]) <= eps &&
                fabs(cur_b - perms[j][1]) <= eps &&
                fabs(cur_c - perms[j][2]) <= eps) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            solve_single_q(eps, cur_a, cur_b, cur_c, &out_solutions[unique_cnt]);
            unique_cnt++;
        }
    }

    *out_unique_count = unique_cnt;
    return SUCCESS;
}