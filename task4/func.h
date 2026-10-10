#ifndef FUNC_H
#define FUNC_H
#include "status.h"

status generate_output_path(const char *in_path, char *out_path, int max_len);
status check_same_file(const char *a, const char *b);
status func_d(const char *in_path, const char *out_path);
status func_i(const char *in_path, const char *out_path);
status func_s(const char *in_path, const char *out_path);
status func_a(const char *in_path, const char *out_path);

#endif