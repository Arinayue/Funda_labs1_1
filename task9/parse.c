#include "parse.h"
#include <limits.h>
#include <stdlib.h>

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