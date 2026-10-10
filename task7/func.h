#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdbool.h>
#include "status.h"

#define MAX_LEXEME_SIZE 1024

status read_token(FILE *stream, char *out_buffer, size_t buffer_size, bool *out_is_eof);
status process_flag_r(const char *path1, const char *path2, const char *path_out);
status process_flag_a(const char *path_in, const char *path_out);
#endif