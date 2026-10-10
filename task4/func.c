#include "func.h"
#include "status.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

static status open_files(const char *in_path, const char *out_path, FILE **in, FILE **out) {
    if (in_path == NULL || out_path == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    *in = fopen(in_path, "r");
    if (*in == NULL) {
        return ERROR_FILE_OPEN;
    }

    *out = fopen(out_path, "w");
    if (*out == NULL) {
        fclose(*in);
        return ERROR_FILE_OPEN;
    }

    return SUCCESS;
}
static status close_files(FILE *in, FILE *out) {
    int read_failed = ferror(in);
    int write_failed = ferror(out);

    fclose(in);
    if (fclose(out) != 0 || write_failed) {
        return ERROR_FILE_WRITE;
    }
    if (read_failed) {
        return ERROR_FILE_READ;
    }
    return SUCCESS;
}
status generate_output_path(const char *in_path, char *out_path, const int max_len) {
    if (in_path == NULL || out_path == NULL || max_len <= 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (in_path[0] == '\0') {
        return ERROR_INVALID_PARAMETER;
    }

    const char *last_slash = strrchr(in_path, '/');
    int written = 0;

    if (last_slash != NULL) {
        int dir_len = (int)(last_slash - in_path + 1);
        const char *filename = last_slash + 1;
        written = snprintf(out_path, max_len, "%.*sout_%s", dir_len, in_path, filename);
    } else {
        written = snprintf(out_path, max_len, "out_%s", in_path);
    }

    if (written < 0 || written >= max_len) {
        return ERROR_INVALID_PARAMETER;
    }

    return SUCCESS;
}

status check_same_file(const char *a, const char *b) {
    if (a == NULL || b == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    if (strcmp(a, b) == 0) {
        return ERROR_SAME_FILE;
    }

    struct stat sa, sb;
    if (stat(a, &sa) == 0 && stat(b, &sb) == 0) {
        if (sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino) {
            return ERROR_SAME_FILE;
        }
    }

    return SUCCESS;
}

status func_d(const char *in_path, const char *out_path) {
    FILE *in, *out;
    status st = open_files(in_path, out_path, &in, &out);
    if (st != SUCCESS) {
        return st;
    }

    int ch;
    while ((ch = fgetc(in)) != EOF) {
        if (ch < '0' || ch > '9') {
            fputc(ch, out);
        }
    }

    return close_files(in, out);
}

status func_i(const char *in_path, const char *out_path) {
    FILE *in, *out;
    status st = open_files(in_path, out_path, &in, &out);
    if (st != SUCCESS) {
        return st;
    }

    int ch;
    int letter_count = 0;
    int line_has_content = 0;

    while ((ch = fgetc(in)) != EOF) {
        if (ch == '\n') {
            fprintf(out, " [букв: %d]\n", letter_count);
            letter_count = 0;
            line_has_content = 0;
        } else {
            line_has_content = 1;
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                letter_count++;
            }
            fputc(ch, out);
        }
    }

    if (line_has_content) {
        fprintf(out, " [букв: %d]", letter_count);
    }

    return close_files(in, out);
}

status func_s(const char *in_path, const char *out_path) {
    FILE *in, *out;
    status st = open_files(in_path, out_path, &in, &out);
    if (st != SUCCESS) {
        return st;
    }

    int ch;
    int spec_count = 0;
    int line_has_content = 0;

    while ((ch = fgetc(in)) != EOF) {
        if (ch == '\n') {
            fprintf(out, " [спецсимволов: %d]\n", spec_count);
            spec_count = 0;
            line_has_content = 0;
        } else {
            line_has_content = 1;

            int is_digit = (ch >= '0' && ch <= '9');
            int is_letter = ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
            int is_space = (ch == ' ');
            int is_continuation = ((ch & 0xC0) == 0x80);

            if (!is_digit && !is_letter && !is_space && !is_continuation) {
                spec_count++;
            }
            fputc(ch, out);
        }
    }

    if (line_has_content) {
        fprintf(out, " [спецсимволов: %d]", spec_count);
    }

    return close_files(in, out);
}

status func_a(const char *in_path, const char *out_path) {
    FILE *in, *out;
    status st = open_files(in_path, out_path, &in, &out);
    if (st != SUCCESS) {
        return st;
    }

    int ch;
    while ((ch = fgetc(in)) != EOF) {
        if ((ch >= '0' && ch <= '9') || ch == '\n') {
            fputc(ch, out);
        } else {
            fprintf(out, "%02X", ch);
        }
    }

    return close_files(in, out);
}