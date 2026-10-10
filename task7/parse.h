#ifndef PARSE_H
#define PARSE_H

#include "status.h"

status parse_flag(const char *str, char *out_flag);
status parse_path(const char *str, char *out_str, const int max_len);
#endif