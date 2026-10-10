#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "parsing.h"

status_code parse_num(const char *str, long *x) {
    if (str == NULL || x == NULL || *str == '\0') {
        return STATUS_INVALID_INPUT;
    }

    char *endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if (str == endptr || *endptr != '\0') {
        return STATUS_INVALID_INPUT;
    }

    if (val == LONG_MAX || val == LONG_MIN) {
        return STATUS_OVERFLOW;
    }

    *x = val;
    return STATUS_OK;
}

status_code parse_flag(const char *str, char *out_flag) {
    if (str == NULL || out_flag == NULL || *str == '\0') {
        return STATUS_INVALID_INPUT;
    }

    if (str[0] != '-' && str[0] != '/') {
        return STATUS_INVALID_INPUT;
    }

    if (strchr("hpseaf", str[1]) == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (str[2] != '\0') {
        return STATUS_INVALID_INPUT;
    }

    *out_flag = str[1];
    return STATUS_OK;
}