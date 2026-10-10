#include "handlers.h"

long poww(const long base, const long exp) {
    long result = 1;
    for (long i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

status_code process_h(const long x, long *out_buffer, int *out_count) {
    if (x > 100 || x < 1) {
        return STATUS_INVALID_RANGE;
    }
    int count = 0;
    for (long i = x; i < 101; i += x) {
        out_buffer[count] = i;
        count += 1;
    }
    *out_count = count;
    if (*out_count == 0) {
        return STATUS_NOT_FOUND;
    }
    return STATUS_OK;
}

status_code process_p(const long x, long *out_buffer, int *out_count) {
    if (x < 2) {
        return STATUS_INVALID_RANGE;
    }
    for (long i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            *out_buffer = 1;
            *out_count = 1;
            return STATUS_OK;
        }
    }
    *out_buffer = 0;
    *out_count = 1;
    return STATUS_OK;
}

status_code process_s(const long x, long *out_buffer, int *out_count) {
    if (x == 0) {
        out_buffer[0] = '0';
        *out_count = 1;
        return STATUS_OK;
    }

    char temp[64];
    int temp_count = 0;

    long n = (x < 0) ? -x : x;

    while (n > 0) {
        int rem = n % 16;
        if (rem < 10) {
            temp[temp_count++] = '0' + rem;
        } else {
            temp[temp_count++] = 'A' + (rem - 10);
        }
        n /= 16;
    }

    int idx = 0;

    if (x < 0) {
        out_buffer[idx++] = '-';
    }

    for (int i = temp_count - 1; i >= 0; i--) {
        out_buffer[idx++] = temp[i];
    }

    *out_count = idx;
    return STATUS_OK;
}

status_code process_e(const long x, long *out_buffer, int *out_count) {
    if (x > 10 || x < 1) {
        return STATUS_INVALID_RANGE;
    }
    
    int idx = 0;
    for (long base = 1; base <= 10; base++) {
        for (long exp = 1; exp <= x; exp++) {
            out_buffer[idx++] = poww(base, exp);
        }
    }
    
    *out_count = idx;
    return STATUS_OK;
}

status_code process_a(const long x, long *out_buffer, int *out_count) {
    if (x < 1) {
        return STATUS_INVALID_RANGE;
    }
    out_buffer[0] = x * (x + 1) / 2;
    *out_count = 1;
    return STATUS_OK;
}

status_code process_f(const long x, long *out_buffer, int *out_count) {
    if (x < 0) {
        return STATUS_INVALID_RANGE;
    }
    long fact = 1;
    for (long i = 1; i <= x; i++) {
        fact *= i;
    }
    out_buffer[0] = fact;
    *out_count = 1;
    return STATUS_OK;
}