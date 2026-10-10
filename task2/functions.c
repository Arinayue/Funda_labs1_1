#include "functions.h"
#include <math.h>
#include <stdlib.h>

#define MAX_ITER 100000000
#define E 2.7182818284590452354
#define PI 3.14159265359

unsigned long long fact(const unsigned long long n) {
    unsigned long long out = 1;
    for (unsigned long long i = 1; i <= n; i++) {
        out *= i;
    }
    return out;
}

status_code e_limit(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double prev = 0.0;
    double current = 0.0;

    for (unsigned long long n = 1; n <= MAX_ITER; n++) {
        current = pow(1.0 + 1.0 / (double)n, (double)n);
        if (n > 1) {
            if (fabs(current - prev) < eps) {
                *out_val = current;
                return STATUS_OK;
            }     
        }
        prev = current;
    }
    return STATUS_MAX_ITERATIONS;
}

status_code e_series(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double current = 1.0;
    double term = 1.0;

    for (unsigned long long n = 1; n <= MAX_ITER; n++) {
        term = term / (double)n;
        current += term;
        
        if (term < eps) {
            *out_val = current;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code e_equation(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double a = 1.0;
    double b = 3.0;
    double mid;

    for (unsigned long long i = 0; i < MAX_ITER; i++) {
        mid = (b + a) / 2.0;

        if (log(mid) - 1.0 > 0.0) {
            b = mid;
        } else {
            a = mid;
        } 
        if ((b - a) < eps) {
            *out_val = mid;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code pi_limit(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }
    double prev = 0.0;
    double current = 4.0;

    for (unsigned long long i = 2; i < MAX_ITER; i++) {
        prev = current;
        double mult = (4.0 * (double)i * ((double)i - 1.0)) / ((2.0 * (double)i - 1.0) * (2.0 * (double)i - 1.0));
        current = current * mult;
        if (fabs(current - prev) < eps) {
            *out_val = current;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code pi_series(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double sum = 0.0;
    double denom = 1.0;
    double sign = 1.0;

    for (unsigned long long i = 1; i < MAX_ITER; i++) {
        double term = sign / denom;
        sum += term;
        if (fabs(4.0 * term) < eps) {
            *out_val = 4.0 * sum;
            return STATUS_OK;
        }
        sign = -sign;
        denom += 2.0;
    } 
    return STATUS_MAX_ITERATIONS;
}

status_code pi_equation(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double a = 2.0;
    double b = 4.0;
    double mid;

    for (unsigned long long i = 0; i < MAX_ITER; i++) {
        mid = (b + a) / 2.0;
        if (sin(mid) > 0.0) {
            a = mid;
        } else {
            b = mid;
        }
        if (fabs(b - a) < eps) {
            *out_val = mid;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code ln2_limit(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }
    double cur = 0.0;
    double prev = 1.0;
    for (unsigned long long i = 2; i < MAX_ITER; i++) {
        cur = (double)i * (pow(2, 1 / (double)i) - 1);
        if (fabs(cur - prev) < eps) {
            *out_val = cur;
            return STATUS_OK;
        }
        prev = cur;
    }
    return STATUS_MAX_ITERATIONS;
}

status_code ln2_series(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double sum = 0.0;
    double sign = 1.0;
    for (unsigned long long i = 1; i < MAX_ITER; i++) {
        double term = sign / (double)i;

        if (fabs(term) < eps) {
            *out_val = sum;
            return STATUS_OK;
        }
        sum += term;
        sign = -sign;
    }
    return STATUS_MAX_ITERATIONS;
}

status_code ln2_equation(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double a = 0.0;
    double b = 1.0;
    for (unsigned long long i = 1; i < MAX_ITER; i++) {
        double mid = (a + b) / 2.0;
        if ((pow(E, mid) - 2) > 0.0) {
            b = mid;
        } else {
            a = mid;
        }
        if (fabs(b - a) < eps) {
            *out_val = mid;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code sqrt2_limit(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double prev = -0.5;
    double cur = 0.0;
    for (unsigned long long i = 1; i < MAX_ITER; i++) {
        cur = prev - (prev * prev) / 2.0 + 1.0;
        if (fabs(cur - prev) < eps) {
            *out_val = cur;
            return STATUS_OK;
        }
        prev = cur;
    }
    return STATUS_MAX_ITERATIONS;
}

status_code sqrt2_series(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }
    double product = 1.0;
    double pow_val = 0.25;

    for (unsigned long long i = 1; i < MAX_ITER; i++) {
        double term = pow(2.0, pow_val);
        if (fabs(term - 1.0) < eps) {
            *out_val = product;
            return STATUS_OK;
        }
        product *= term;
        pow_val /= 2.0;
    }
    return STATUS_MAX_ITERATIONS;
}   

status_code sqrt2_equation(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double a = 1.0;
    double b = 2.0;
    double mid;
    for (unsigned long long i = 0; i < MAX_ITER; i++) {
        mid = (a + b) / 2.0;
        if (mid * mid - 2.0 > 0.0) {
            b = mid;
        } else {
            a = mid;
        }
        if (fabs(b - a) < eps) {
            *out_val = mid;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

status_code gamma_limit(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double s_prev = 0.0;

    for (unsigned long long m = 1; m < MAX_ITER; m++) {
        double s_curr = 0.0;
        double c_m_k = (double)m; 
        double ln_fact = 0.0;     
        double sign = -1.0;       

        for (unsigned long long k = 1; k <= m; k++) {
            ln_fact += log((double)k); 
            
            double term = c_m_k * (sign / (double)k) * ln_fact;
            s_curr += term;

            c_m_k = c_m_k * (double)(m - k) / (double)(k + 1);
            sign = -sign;
        }

        if (m > 1 && fabs(s_curr - s_prev) < eps) {
            *out_val = s_curr;
            return STATUS_OK;
        }

        s_prev = s_curr;
    }

    return STATUS_MAX_ITERATIONS;
}

status_code gamma_series(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double sum = -(PI * PI) / 6.0;
    for (unsigned long long i = 2; i < MAX_ITER; i++) {
        double floor_sqrt = floor(sqrt((double)i));
        double term = 1.0 / (floor_sqrt * floor_sqrt) - 1.0 / (double)i;
        sum += term;

        if (fabs(term) < eps) {
            *out_val = sum;
            return STATUS_OK;
        }
    }
    return STATUS_MAX_ITERATIONS;
}

static int is_prime(const unsigned long long n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (unsigned long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

status_code gamma_equation(const double eps, double* out_val) {
    if (out_val == NULL) {
        return STATUS_INVALID_INPUT;
    }

    if (eps <= 0.0 || eps >= 1.0) {
        return STATUS_INVALID_INPUT;
    }

    double product = 1.0;
    double x_prev = 0.0;

    for (unsigned long long t = 2; t < MAX_ITER; t++) {
        if (is_prime(t)) {
            product *= (double)(t - 1) / (double)t;
        }

        double L_t = log((double)t) * product;
        double x_curr = -log(L_t);

        if (t > 2 && fabs(x_curr - x_prev) < eps) {
            *out_val = x_curr;
            return STATUS_OK;
        }

        x_prev = x_curr;
    }

    return STATUS_MAX_ITERATIONS;
}