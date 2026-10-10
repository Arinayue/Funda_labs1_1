#include "func.h"
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

status char_to_digit(const char ch,const long base,long long* out_val) {
    long long digit = -1;

    if (isdigit(ch)) {
        digit = ch - '0';
    } else if (isupper(ch)) {
        digit = ch - 'A' + 10;
    } else {
        return ERROR_INVALID_ARGUMENTS; 
    }

    if (digit >= base) {
        return ERROR_INVALID_ARGUMENTS; 
    }
    *out_val = digit;
    return SUCCESS;
}

status str_to_num(const char* str,const long base, long long* out_val){
    if (str == NULL || out_val == NULL || *str == '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    const char *pch = str;
    long long res = 0;
    int sign = 1;
    if (*pch == '-') {
        sign = -1;
        pch++;
    } else if (*pch == '+') {
        pch++;
    }

    if (*pch == '-' || *pch == '+' || *pch == '\0') {
        return ERROR_INVALID_ARGUMENTS;
    }

    while (*pch != '\0') {
        long long digit;
        status err = char_to_digit(*pch, base, &digit);
        if (err != SUCCESS) {
            return err;
        }


        if (res < ((LLONG_MIN + digit) / base)) {
            return ERROR_OVERFLOW;
        } else {
            res = res * base - digit;
        }

        
        pch++;
    }
    if (sign == 1) {
        if (res == LLONG_MIN) {
            return ERROR_OVERFLOW; 
        }
        res = -res;
    }
    *out_val = res;
    
    return SUCCESS;
}

status num_to_str(const long long value, const long base, char *buf, const int buf_size) {
    if (buf == NULL || buf_size < 2 || base < 2 || base > 36) {
        return ERROR_INVALID_ARGUMENTS;
    }

    int idx = buf_size - 1;
    buf[idx] = '\0';

    int is_negative = (value < 0);
    long long temp = is_negative ? value : -value; 

    do {
        if (idx <= (is_negative ? 1 : 0)) {
            return ERROR_OVERFLOW; 
        }

        int remainder = -(temp % base);
        char digit_char;

        if (remainder < 10) {
            digit_char = '0' + remainder;
        } else {
            digit_char = 'A' + (remainder - 10);
        }

        buf[--idx] = digit_char;
        temp /= base;
    } while (temp != 0);

    if (is_negative) {
        buf[--idx] = '-';
    }

    int i = 0;
    while (buf[idx] != '\0') {
        buf[i++] = buf[idx++];
    }
    buf[i] = '\0';

    return SUCCESS;
}
