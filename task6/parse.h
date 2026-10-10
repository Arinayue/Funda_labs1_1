#ifndef PARSE_H
#define PARSE_H

#include "status.h"

status parse_ull_base(const char *s, int base, unsigned long long *out);

#endif