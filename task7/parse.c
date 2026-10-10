#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include "status.h"


status parse_flag(const char *str, char *out_flag) {
    if (str == NULL || out_flag == NULL || *str == '\0') {
        return ERROR_INVALID_PARAMETER;
    }

    if (str[0] != '-' && str[0] != '/') {
        return ERROR_INVALID_PARAMETER;
    }

    if (strchr("ra", str[1]) == NULL) {
        return ERROR_UNKNOWN_FLAG;
    }

    if (str[2] != '\0') {
        return ERROR_INVALID_PARAMETER;
    }

    *out_flag = str[1];
    return SUCCESS;
}


status parse_path(const char *str, char *out_str, const int max_len) {
    if (str == NULL || out_str == NULL || max_len <= 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (str[0] == '\0' || (int)strlen(str) >= max_len) {
        return ERROR_INVALID_PARAMETER;
    }

    const char *slash_pos = strrchr(str, '/');
    const char *filename = (slash_pos != NULL) ? (slash_pos + 1) : str;

    const char *ext = strrchr(filename, '.');

    if (ext == NULL || ext == filename || ext[1] == '\0') {
        return ERROR_INVALID_PARAMETER;
    }

    strcpy(out_str, str);

    return SUCCESS;
}