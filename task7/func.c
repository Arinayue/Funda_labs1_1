#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "func.h"
#include <string.h>

status read_token(FILE *stream, char *out_buffer, size_t buffer_size, bool *out_is_eof) {
    if (stream == NULL || out_buffer == NULL || out_is_eof == NULL || buffer_size < 2) {
        return ERROR_INVALID_PARAMETER;
    }

    *out_is_eof = false;
    out_buffer[0] = '\0';

    int ch = fgetc(stream);

    while (ch != EOF && isspace((unsigned char)ch)) {
        ch = fgetc(stream);
    }

    if (ch == EOF) {
        *out_is_eof = true;
        return SUCCESS;
    }

    size_t length = 0;
    while (ch != EOF && !isspace((unsigned char)ch)) {
        if (length >= buffer_size - 1) {
            return ERROR_FILE_READ;
        }
        out_buffer[length++] = (char)ch;
        ch = fgetc(stream);
    }

    out_buffer[length] = '\0';

    return SUCCESS;
}

status process_flag_r(const char *path1, const char *path2, const char *path_out) {
    if (path1 == NULL || path2 == NULL || path_out == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    if (strcmp(path1, path_out) == 0 || strcmp(path2, path_out) == 0) {
        return ERROR_SAME_FILE;
    }

    FILE *f1 = fopen(path1, "r");
    if (f1 == NULL) return ERROR_FILE_OPEN;

    FILE *f2 = fopen(path2, "r");
    if (f2 == NULL) {
        fclose(f1);
        return ERROR_FILE_OPEN;
    }

    FILE *fout = fopen(path_out, "w");
    if (fout == NULL) {
        fclose(f1);
        fclose(f2);
        return ERROR_FILE_OPEN;
    }

    char token[MAX_LEXEME_SIZE];
    bool eof1 = false, eof2 = false;
    bool is_first = true;
    status st = SUCCESS;

    while (!eof1 || !eof2) {
        if (!eof1) {
            st = read_token(f1, token, sizeof(token), &eof1);
            if (st != SUCCESS) break;
            if (!eof1) {
                if (!is_first) fputc(' ', fout);
                if (fputs(token, fout) == EOF) { st = ERROR_FILE_WRITE; break; }
                is_first = false;
            }
        }

        if (!eof2) {
            st = read_token(f2, token, sizeof(token), &eof2);
            if (st != SUCCESS) break;
            if (!eof2) {
                if (!is_first) fputc(' ', fout);
                if (fputs(token, fout) == EOF) { st = ERROR_FILE_WRITE; break; }
                is_first = false;
            }
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(fout);

    return st;
}

static void char_to_base(unsigned char ch, int base, char *out_buf, size_t out_size) {
    if (ch == 0) {
        snprintf(out_buf, out_size, "0");
        return;
    }

    char temp[16];
    int idx = 0;
    unsigned int val = ch;

    while (val > 0) {
        temp[idx++] = (char)('0' + (val % base));
        val /= base;
    }

    size_t out_idx = 0;
    while (idx > 0 && out_idx < out_size - 1) {
        out_buf[out_idx++] = temp[--idx];
    }
    out_buf[out_idx] = '\0';
}

static status transform_lexeme(const char *in_token, size_t index, char *out_buf, size_t out_size) {
    if (in_token == NULL || out_buf == NULL) return ERROR_INVALID_PARAMETER;

    out_buf[0] = '\0';
    size_t out_pos = 0;

    if (index % 10 == 0) {
        for (size_t i = 0; in_token[i] != '\0'; ++i) {
            unsigned char c = (unsigned char)tolower((unsigned char)in_token[i]);
            char base4_str[8];
            char_to_base(c, 4, base4_str, sizeof(base4_str));

            size_t len = strlen(base4_str);
            if (out_pos + len >= out_size - 1) return ERROR_MEMORY_ALLOCATION; 
            strcpy(out_buf + out_pos, base4_str);
            out_pos += len;
        }
    }
    else if (index % 5 == 0) {
        for (size_t i = 0; in_token[i] != '\0'; ++i) {
            unsigned char c = (unsigned char)in_token[i];
            char base8_str[8];
            char_to_base(c, 8, base8_str, sizeof(base8_str));

            size_t len = strlen(base8_str);
            if (out_pos + len >= out_size - 1) return ERROR_MEMORY_ALLOCATION;
            strcpy(out_buf + out_pos, base8_str);
            out_pos += len;
        }
    }
    else if (index % 2 == 0) {
        for (size_t i = 0; in_token[i] != '\0'; ++i) {
            if (out_pos >= out_size - 1) return ERROR_MEMORY_ALLOCATION;
            out_buf[out_pos++] = (char)tolower((unsigned char)in_token[i]);
        }
        out_buf[out_pos] = '\0';
    }
    else {
        if (strlen(in_token) >= out_size) return ERROR_MEMORY_ALLOCATION;
        strcpy(out_buf, in_token);
    }

    return SUCCESS;
}

status process_flag_a(const char *path_in, const char *path_out) {
    if (path_in == NULL || path_out == NULL) return ERROR_INVALID_PARAMETER;
    if (strcmp(path_in, path_out) == 0) return ERROR_SAME_FILE;

    FILE *fin = fopen(path_in, "r");
    if (fin == NULL) return ERROR_FILE_OPEN;

    FILE *fout = fopen(path_out, "w");
    if (fout == NULL) {
        fclose(fin);
        return ERROR_FILE_OPEN;
    }

    char token[MAX_LEXEME_SIZE];
    char transformed[MAX_LEXEME_SIZE * 4]; 
    bool is_eof = false;
    bool is_first = true;
    size_t count = 1;
    status st = SUCCESS;

    while (1) {
        st = read_token(fin, token, sizeof(token), &is_eof);
        if (st != SUCCESS || is_eof) break;

        st = transform_lexeme(token, count, transformed, sizeof(transformed));
        if (st != SUCCESS) break;

        if (!is_first) fputc(' ', fout);
        if (fputs(transformed, fout) == EOF) {
            st = ERROR_FILE_WRITE;
            break;
        }

        is_first = false;
        count++;
    }

    fclose(fin);
    fclose(fout);

    return st;
}