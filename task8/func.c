#include "func.h"
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdbool.h>

static bool is_same_file(const char *path_a, const char *path_b) {
    struct stat sa, sb;

    if (stat(path_a, &sa) != 0 || stat(path_b, &sb) != 0) {
        return false; 
    }

    return sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino;
}

status read_token(FILE *stream, char *out_buffer, size_t buffer_size, bool *out_is_eof) {
    if (stream == NULL || out_buffer == NULL || out_is_eof == NULL || buffer_size < 2) {
        return ERROR_INVALID_PARAMETER;
    }

    *out_is_eof = false;
    out_buffer[0] = '\0';

    int ch = fgetc(stream);

    while (ch != EOF && isspace((unsigned char)ch)) {
        ch = fgetc(stream);
    }

    if (ch == EOF) {
        *out_is_eof = true;
        return SUCCESS;
    }

    size_t length = 0;
    bool overflow = false;

    while (ch != EOF && !isspace((unsigned char)ch)) {
        if (length < buffer_size - 1) {
            out_buffer[length++] = (char)ch;
        } else {
            overflow = true;
        }
        ch = fgetc(stream);
    }

    if (overflow) {
        out_buffer[0] = '\0';
        return ERROR_TOKEN_TOO_LONG;
    }

    out_buffer[length] = '\0';
    return SUCCESS;
}

status char_to_digit(const char ch, const long base, long long *out_val) {
    if (out_val == NULL || base < 2 || base > 36) {
        return ERROR_INVALID_PARAMETER;
    }

    long long digit = -1;

    if (isdigit((unsigned char)ch)) {
        digit = ch - '0';
    } else if (isupper((unsigned char)ch)) {
        digit = ch - 'A' + 10;
    } else if (islower((unsigned char)ch)) {
        digit = ch - 'a' + 10;
    } else {
        return ERROR_INVALID_PARAMETER;
    }

    if (digit >= base) {
        return ERROR_INVALID_PARAMETER;
    }

    *out_val = digit;
    return SUCCESS;
}

status find_min_base(const char *str, int *out_min_base) {
    if (str == NULL || out_min_base == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') {
        start = 1;
    }

    if (str[start] == '\0') {
        return ERROR_INVALID_NUMBER_FORMAT;
    }

    int max_digit = 1;

    for (size_t i = start; str[i] != '\0'; ++i) {
        long long digit_val = 0;
        if (char_to_digit(str[i], 36, &digit_val) != SUCCESS) {
            return ERROR_INVALID_NUMBER_FORMAT;
        }
        if (digit_val > max_digit) {
            max_digit = (int)digit_val;
        }
    }

    *out_min_base = max_digit + 1;
    return SUCCESS;
}

status normalize_number(const char *src, char *dst, size_t dst_size, bool *out_is_negative) {
    if (src == NULL || dst == NULL || out_is_negative == NULL || dst_size < 2) {
        return ERROR_INVALID_PARAMETER;
    }

    *out_is_negative = false;
    size_t idx = 0;

    if (src[idx] == '-') {
        *out_is_negative = true;
        idx++;
    } else if (src[idx] == '+') {
        idx++;
    }

    while (src[idx] == '0') {
        idx++;
    }

    if (src[idx] == '\0') {
        if (dst_size < 2) return ERROR_TOKEN_TOO_LONG;
        dst[0] = '0';
        dst[1] = '\0';
        *out_is_negative = false;
        return SUCCESS;
    }

    size_t out_idx = 0;
    while (src[idx] != '\0') {
        if (out_idx >= dst_size - 1) {
            return ERROR_TOKEN_TOO_LONG;
        }
        dst[out_idx++] = src[idx++];
    }

    dst[out_idx] = '\0';
    return SUCCESS;
}

status parse_to_decimal(const char *str, int base, long long *out_value) {
    if (str == NULL || out_value == NULL || base < 2 || base > 36) {
        return ERROR_INVALID_PARAMETER;
    }

    long long result = 0;

    for (size_t i = 0; str[i] != '\0'; ++i) {
        long long digit = 0;
        status st = char_to_digit(str[i], base, &digit);
        if (st != SUCCESS) {
            return st;
        }

        if (result > (LLONG_MAX - digit) / base) {
            return ERROR_OVERFLOW;
        }

        result = result * base + digit;
    }

    *out_value = result;
    return SUCCESS;
}

status process_files(const char *input_path, const char *output_path) {
    
    if (is_same_file(input_path, output_path)) {
        return ERROR_SAME_FILE;
    }
    if (input_path == NULL || output_path == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    FILE *in = fopen(input_path, "r");
    if (in == NULL) {
        return ERROR_FILE_OPEN;
    }

    FILE *out = fopen(output_path, "w");
    if (out == NULL) {
        fclose(in);
        return ERROR_FILE_OPEN;
    }

    char raw_token[MAX_TOKEN_LEN];
    char normalized[MAX_TOKEN_LEN];
    bool is_eof = false;
    status st = SUCCESS;

    while (true) {
        st = read_token(in, raw_token, sizeof(raw_token), &is_eof);
        if (st != SUCCESS) {
            break;
        }

        if (is_eof) {
            st = SUCCESS;
            break;
        }

        int min_base = 0;
        st = find_min_base(raw_token, &min_base);
        if (st != SUCCESS) {
            break;
        }

        bool is_negative = false;
        st = normalize_number(raw_token, normalized, sizeof(normalized), &is_negative);
        if (st != SUCCESS) {
            break;
        }

        long long dec_value = 0;
        st = parse_to_decimal(normalized, min_base, &dec_value);
        if (st != SUCCESS) {
            break;
        }

        if (fprintf(out, "%s%s %d %s%lld\n", 
                    is_negative ? "-" : "", 
                    normalized, 
                    min_base, 
                    is_negative ? "-" : "", 
                    dec_value) < 0) {
            st = ERROR_FILE_WRITE;
            break;
        }
    }

    fclose(in);
    fclose(out);

    return st;
}