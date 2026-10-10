#include <stdlib.h>
#include <math.h>
#include "parse.h"

status parse_double(const char *str, double *out_val) {
    if (str == NULL || out_val == NULL || *str == '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    char *endptr = NULL;
    double val = strtod(str, &endptr);

    if (str == endptr || *endptr != '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (val == HUGE_VAL || val == -HUGE_VAL || isnan(val) || isinf(val)) {
        return ERROR_INVALID_ARGUMENTS;
    }

    *out_val = val;
    return SUCCESS;
}