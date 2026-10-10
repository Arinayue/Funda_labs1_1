#include "func.h"
#include <stdlib.h>
#include <string.h>


status rdm_arr_mn_mx(const long a, const long b, long* nums, long size_nums) {
    if (nums == NULL || size_nums <= 0 || a > b) {
        return ERROR_INVALID_ARGUMENTS;
    }

    unsigned long long range = (unsigned long long)b - (unsigned long long)a + 1ULL;

    long idx_mn = 0;
    long idx_mx = 0;

    for (long i = 0; i < size_nums; i++) {
        long num = a + (long)(rand() % range);
        nums[i] = num;

        if (i == 0) {
            idx_mn = 0;
            idx_mx = 0;
        } else {
            if (num < nums[idx_mn]) {
                idx_mn = i;
            }
            if (num > nums[idx_mx]) {
                idx_mx = i;
            }
        }
    }

    long temp = nums[idx_mn];
    nums[idx_mn] = nums[idx_mx];
    nums[idx_mx] = temp;

    return SUCCESS;
}

static int cmp_long(const void* x, const void* y) {
    long p = *(const long*)x;
    long q = *(const long*)y;
    return (p > q) - (p < q);
}

static long find_closest_val(long val, const long* sorted_b, long size_b) {
    long left = 0;
    long right = size_b;

    while (left < right) {
        long mid = left + (right - left) / 2;
        if (sorted_b[mid] < val) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    if (left == 0) {
        return sorted_b[0];
    }
    if (left == size_b) {
        return sorted_b[size_b - 1];
    }

    long diff_left = val - sorted_b[left - 1];
    long diff_right = sorted_b[left] - val;

    return (diff_left <= diff_right) ? sorted_b[left - 1] : sorted_b[left];
}

status create_arrays_a_b_c(long** out_a, long* out_size_a, long** out_b, long* out_size_b, long** out_c) {
    if (out_a == NULL || out_size_a == NULL ||
        out_b == NULL || out_size_b == NULL ||
        out_c == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }

    long size_a = 10 + rand() % (10000 - 10 + 1);
    long size_b = 10 + rand() % (10000 - 10 + 1);

    long* a = (long*)malloc(sizeof(long) * size_a);
    if (a == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    long* b = (long*)malloc(sizeof(long) * size_b);
    if (b == NULL) {
        free(a);
        return ERROR_MEMORY_ALLOCATION;
    }

    long* c = (long*)malloc(sizeof(long) * size_a);
    if (c == NULL) {
        free(a);
        free(b);
        return ERROR_MEMORY_ALLOCATION;
    }

    long* sorted_b = (long*)malloc(sizeof(long) * size_b);
    if (sorted_b == NULL) {
        free(a);
        free(b);
        free(c);
        return ERROR_MEMORY_ALLOCATION;
    }

    for (long i = 0; i < size_a; i++) {
        a[i] = -1000 + rand() % (1000 - (-1000) + 1);
    }
    for (long i = 0; i < size_b; i++) {
        b[i] = -1000 + rand() % (1000 - (-1000) + 1);
    }

    /* сортируем копию, чтобы исходный B остался как был */
    memcpy(sorted_b, b, sizeof(long) * size_b);
    qsort(sorted_b, size_b, sizeof(long), cmp_long);

    for (long i = 0; i < size_a; i++) {
        long closest = find_closest_val(a[i], sorted_b, size_b);
        c[i] = a[i] + closest;
    }

    free(sorted_b);

    *out_a = a;
    *out_b = b;
    *out_c = c;
    *out_size_a = size_a;
    *out_size_b = size_b;

    return SUCCESS;
}