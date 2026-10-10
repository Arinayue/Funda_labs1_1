#include "parse.h"
#include <string.h>
#include "status.h"

status parse_flag(const char *str, char *out_flag) {
    if (str == NULL || out_flag == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    int len = (int)strlen(str);
    if (len != 2 && len != 3) {
        return ERROR_INVALID_PARAMETER;
    }

    if (str[0] != '-' && str[0] != '/') {
        return ERROR_INVALID_PARAMETER;
    }

    int action_idx = 1;

    if (len == 3) {
        if (str[1] != 'n') {
            return ERROR_INVALID_PARAMETER;
        }
        action_idx = 2;
    }

    if (strchr("disa", str[action_idx]) == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    strcpy(out_flag, str);

    return SUCCESS;
}

status parse_path(const char *str, char *out_str, const int max_len) {
    if (str == NULL || out_str == NULL || max_len <= 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (str[0] == '\0') {
        return ERROR_INVALID_PARAMETER;
    }

    if ((int)strlen(str) >= max_len) {
        return ERROR_INVALID_PARAMETER;
    }

    strcpy(out_str, str);

    return SUCCESS;
}