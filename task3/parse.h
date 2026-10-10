#ifndef PARSE_H
#define PARSE_H
#include "status.h"

status parse_flag(const char *arg, char *out_flag);
status parse_double(const char *str, double *out_value);
status parse_long(const char *str, long *out_value);

#endif