#ifndef PARSING_H
#define PARSING_H

#include "status.h"
status_code parse_num(const char *str, long *x);

status_code parse_flag(const char *str, char *out_flag);

#endif