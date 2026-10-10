#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "status.h"

#define MAX_TOKEN_LEN 256

status read_token(FILE *stream, char *out_buffer, size_t buffer_size, bool *out_is_eof);
status char_to_digit(const char ch, const long base, long long *out_val);
status find_min_base(const char *str, int *out_min_base);
status normalize_number(const char *src, char *dst, size_t dst_size, bool *out_is_negative);
status parse_to_decimal(const char *str, int base, long long *out_value);
status process_files(const char *input_path, const char *output_path);

#endif