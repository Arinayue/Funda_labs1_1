#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "status.h"

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

status parse_long(const char *str, long *out_val) {
    if (str == NULL || out_val == NULL || *str == '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    char *endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if (str == endptr || *endptr != '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (val == LONG_MAX || val == LONG_MIN) {
        return ERROR_INVALID_ARGUMENTS;
    }

    *out_val = val;
    return SUCCESS;
}

status parse_flag(const char *arg, char *out_flag) {
    if (arg == NULL || out_flag == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (arg[0] != '-' && arg[0] != '/') {
        return ERROR_INVALID_ARGUMENTS;
    }

    if (strchr("qmt", arg[1]) == NULL) {
        return ERROR_UNKNOWN_FLAG;
    }

    if (arg[2] != '\0') {
        return ERROR_UNKNOWN_FLAG;
    }

    *out_flag = arg[1];
    return SUCCESS;
}