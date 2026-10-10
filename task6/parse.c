#include <limits.h>
#include <stddef.h>

#include "parse.h"

static int digit_value(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    return -1;
}

status parse_ull_base(const char *const s, const int base,
                      unsigned long long *const out)
{
    if (s == NULL || out == NULL || base < 2 || base > 36 || s[0] == '\0')
        return ERROR_INVALID_ARGUMENTS;

    unsigned long long value = 0;
    for (size_t i = 0; s[i] != '\0'; ++i)
    {
        const int d = digit_value(s[i]);
        if (d < 0 || d >= base)
            return ERROR_INVALID_ARGUMENTS;
        if (value > (ULLONG_MAX - (unsigned long long)d) / (unsigned long long)base)
            return ERROR_OVERFLOW;
        value = value * (unsigned long long)base + (unsigned long long)d;
    }

    *out = value;
    return SUCCESS;
}