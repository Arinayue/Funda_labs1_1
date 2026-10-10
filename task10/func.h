#ifndef FUNC_H
#define FUNC_H

#include "status.h"


status char_to_digit(const char ch,const long base,long long* out_val);

status str_to_num(const char* str,const long base, long long* out_val);

status num_to_str(const long long value, const long base, char *buf, int buf_size);

#endif