#include <stdlib.h>
#include <math.h>
#include "status.h"

status_code parse_eps(const char* str, double* eps) {
    if (str == NULL || eps == NULL || *str == '\0') {
        return STATUS_INVALID_INPUT;
    }

    char *endptr = NULL;
    double val = strtod(str, &endptr);

    if (str == endptr || *endptr != '\0') {
        return STATUS_INVALID_INPUT;
    }

    if (val == HUGE_VAL || val == -HUGE_VAL || isnan(val) || isinf(val)) {
        return STATUS_OVERFLOW;
    }

    if (val <= 0.0 || val >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    *eps = val;
    return STATUS_OK;
}