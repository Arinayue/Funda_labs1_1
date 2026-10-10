#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#include "func.h"
#include "parse.h"

#define PI 3.14159265358979323846
#define BISECT_MAX_ITERATIONS 10000

typedef struct
{
    double x;
    double y;
} Point2D;

static status check_convex(const Point2D *const pts, const int n,
                           const double eps, int *const result)
{
    int sign = 0;
    int convex = 1;
    double total_angle = 0.0;

    for (int i = 0; i < n; ++i)
    {
        const Point2D p0 = pts[i];
        const Point2D p1 = pts[(i + 1) % n];
        const Point2D p2 = pts[(i + 2) % n];

        const double e1x = p1.x - p0.x, e1y = p1.y - p0.y;
        const double e2x = p2.x - p1.x, e2y = p2.y - p1.y;

        if (fabs(e1x) <= eps && fabs(e1y) <= eps)
            return ERROR_NO_RESULT;

        const double cross = e1x * e2y - e1y * e2x;
        const double dot = e1x * e2x + e1y * e2y;

        if (cross > eps)
        {
            if (sign == -1)
                convex = 0;
            sign = 1;
        }
        else if (cross < -eps)
        {
            if (sign == 1)
                convex = 0;
            sign = -1;
        }
        else if (dot < 0.0)
        {
            convex = 0;
        }

        total_angle += atan2(cross, dot);
    }

    if (sign == 0)
        return ERROR_NO_RESULT;

    if (fabs(fabs(total_angle) - 2.0 * PI) > eps)
        convex = 0;

    *result = convex;
    return SUCCESS;
}

status is_convex_polygon(int *const result, const double eps, const int n, ...)
{
    if (result == NULL || !(eps > 0.0) || n < 3)
        return ERROR_INVALID_ARGUMENTS;

    Point2D *const pts = malloc((size_t)n * sizeof *pts);
    if (pts == NULL)
        return ERROR_MEMORY_ALLOCATION;

    va_list args;
    va_start(args, n);
    status st = SUCCESS;
    for (int i = 0; i < n; ++i)
    {
        pts[i].x = va_arg(args, double);
        pts[i].y = va_arg(args, double);
        if (!isfinite(pts[i].x) || !isfinite(pts[i].y))
        {
            st = ERROR_INVALID_ARGUMENTS;
            break;
        }
    }
    va_end(args);

    if (st == SUCCESS)
        st = check_convex(pts, n, eps, result);

    free(pts);
    return st;
}

status poly_value(double *const result, const double x, const int n, ...)
{
    if (result == NULL || !isfinite(x) || n < 0)
        return ERROR_INVALID_ARGUMENTS;

    va_list args;
    va_start(args, n);

    double acc = 0.0;
    status st = SUCCESS;
    for (int i = 0; i <= n; ++i)
    {
        const double coef = va_arg(args, double);
        if (!isfinite(coef))
        {
            st = ERROR_INVALID_ARGUMENTS;
            break;
        }
        acc = acc * x + coef;
        if (!isfinite(acc))
        {
            st = ERROR_OVERFLOW;
            break;
        }
    }
    va_end(args);

    if (st == SUCCESS)
        *result = acc;
    return st;
}

static status is_kaprekar(int *const result, const unsigned long long n,
                          const int base)
{
    if (n == 0)
    {
        *result = 0;
        return SUCCESS;
    }
    if (n > 0xFFFFFFFFULL)
        return ERROR_OVERFLOW;

    const unsigned long long sq = n * n;
    const unsigned long long b = (unsigned long long)base;

    int found = 0;
    unsigned long long p = b;
    for (;;)
    {
        const unsigned long long c = sq % p;
        const unsigned long long a = sq / p;
        if (c > 0 && a + c == n)
        {
            found = 1;
            break;
        }
        if (p > sq / b)
            break;
        p *= b;
    }

    *result = found;
    return SUCCESS;
}

status find_kaprekar(const char **const found, size_t *const found_count,
                     const int base, const int count, ...)
{
    if (found == NULL || found_count == NULL || base < 2 || base > 36 || count <= 0)
        return ERROR_INVALID_ARGUMENTS;

    va_list args;
    va_start(args, count);

    size_t cnt = 0;
    status st = SUCCESS;
    for (int i = 0; i < count && st == SUCCESS; ++i)
    {
        const char *const s = va_arg(args, const char *);
        unsigned long long value = 0;
        int is_kap = 0;

        st = parse_ull_base(s, base, &value);
        if (st == SUCCESS)
            st = is_kaprekar(&is_kap, value, base);
        if (st == SUCCESS && is_kap)
            found[cnt++] = s;
    }
    va_end(args);

    *found_count = (st == SUCCESS) ? cnt : 0;
    return st;
}

status geometric_mean(double *const result, const double eps, const int count, ...)
{
    if (result == NULL || !(eps > 0.0) || count <= 0)
        return ERROR_INVALID_ARGUMENTS;

    va_list args;
    va_start(args, count);

    double log_sum = 0.0;
    int has_zero = 0;
    status st = SUCCESS;
    for (int i = 0; i < count; ++i)
    {
        const double v = va_arg(args, double);
        if (!isfinite(v) || v < -eps)
        {
            st = ERROR_INVALID_ARGUMENTS;
            break;
        }
        if (v <= eps)
            has_zero = 1;
        else
            log_sum += log(v);
    }
    va_end(args);

    if (st != SUCCESS)
        return st;

    if (has_zero)
    {
        *result = 0.0;
        return SUCCESS;
    }

    const double g = exp(log_sum / (double)count);
    if (!isfinite(g))
        return ERROR_OVERFLOW;

    *result = g;
    return SUCCESS;
}

static double pow_rec(const double base, const unsigned long long e)
{
    if (e == 0)
        return 1.0;
    const double half = pow_rec(base, e / 2);
    return (e % 2 == 0) ? half * half : half * half * base;
}

status fast_pow(double *const result, const double base, const int exp,
                const double eps)
{
    if (result == NULL || !(eps > 0.0) || !isfinite(base))
        return ERROR_INVALID_ARGUMENTS;

    if (fabs(base) <= eps)
    {
        if (exp <= 0)
            return ERROR_NO_RESULT;
        *result = 0.0;
        return SUCCESS;
    }

    const long long e = (long long)exp;
    const unsigned long long abs_e = (unsigned long long)(e < 0 ? -e : e);

    double r = pow_rec(base, abs_e);
    if (!isfinite(r))
        return ERROR_OVERFLOW;

    if (exp < 0)
    {
        if (r == 0.0)
            return ERROR_OVERFLOW;
        r = 1.0 / r;
        if (!isfinite(r))
            return ERROR_OVERFLOW;
    }

    *result = r;
    return SUCCESS;
}

static int sign_of(const double v)
{
    return (v > 0.0) - (v < 0.0);
}

status bisect_root(double *const root, const double a, const double b,
                   const double eps, double (*const f)(double))
{
    if (root == NULL || f == NULL || !(eps > 0.0) ||
        !isfinite(a) || !isfinite(b) || !(a < b))
        return ERROR_INVALID_ARGUMENTS;

    double left = a, right = b;
    const double f_left = f(left);
    const double f_right = f(right);
    if (!isfinite(f_left) || !isfinite(f_right))
        return ERROR_INVALID_ARGUMENTS;

    const int s_left = sign_of(f_left);
    const int s_right = sign_of(f_right);

    if (s_left == 0)
    {
        *root = left;
        return SUCCESS;
    }
    if (s_right == 0)
    {
        *root = right;
        return SUCCESS;
    }
    if (s_left == s_right)
        return ERROR_NO_RESULT;

    for (int i = 0; i < BISECT_MAX_ITERATIONS && (right - left) > eps; ++i)
    {
        const double mid = left + (right - left) / 2.0;
        const double f_mid = f(mid);
        if (!isfinite(f_mid))
            return ERROR_INVALID_ARGUMENTS;

        const int s_mid = sign_of(f_mid);
        if (s_mid == 0)
        {
            *root = mid;
            return SUCCESS;
        }
        if (s_mid == s_left)
            left = mid;
        else
            right = mid;
    }

    *root = left + (right - left) / 2.0;
    return SUCCESS;
}