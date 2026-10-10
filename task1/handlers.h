#ifndef HANDLERS_H
#define HANDLERS_H

#include "status.h"

typedef status_code (*flag_handler)(const long x, long *out_buffer, int *out_count);

typedef struct {
    char flag;
    flag_handler handler;
} FlagMap;

long poww(const long base, const long exp);

status_code process_h(const long x, long *out_buffer, int *out_count);
status_code process_p(const long x, long *out_buffer, int *out_count);
status_code process_s(const long x, long *out_buffer, int *out_count);
status_code process_e(const long x, long *out_buffer, int *out_count);
status_code process_a(const long x, long *out_buffer, int *out_count);
status_code process_f(const long x, long *out_buffer, int *out_count);

#endif