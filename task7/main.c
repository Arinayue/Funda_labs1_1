#include <stdio.h>
#include "status.h"
#include "parse.h"
#include "func.h"

#define MAX_PATH_LEN 1024

static void print_error(const status st) {
    switch (st) {
        case ERROR_INVALID_PARAMETER:
            printf("Ошибка: неверные параметры командной строки или формат пути.\n");
            break;
        case ERROR_UNKNOWN_FLAG:
            printf("Ошибка: неизвестный флаг. Поддерживаются только -r, /r, -a, /a.\n");
            break;
        case ERROR_FILE_OPEN:
            printf("Ошибка: не удалось открыть один из указанных файлов.\n");
            break;
        case ERROR_FILE_READ:
            printf( "Ошибка при чтении из файла (возможно, переполнение буфера лексемы).\n");
            break;
        case ERROR_FILE_WRITE:
            printf("Ошибка записи в выходной файл.\n");
            break;
        case ERROR_SAME_FILE:
            printf("Ошибка: входной и выходной файлы не могут совпадать.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка перевыделения / недостатка памяти.\n");
            break;
        default:
            printf("Произошла неизвестная ошибка (код: %d).\n", st);
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Использование:\n");
        fprintf(stderr, "  %s -r <file1> <file2> <out_file>\n", argv[0]);
        fprintf(stderr, "  %s -a <in_file> <out_file>\n", argv[0]);
        return 1;
    }

    char flag = '\0';
    status st = parse_flag(argv[1], &flag);
    if (st != SUCCESS) {
        print_error(st);
        return 1;
    }

    if (flag == 'r') {
        if (argc != 5) {
            fprintf(stderr, "Ошибка: для флага -r требуется ровно 3 пути к файлам.\n");
            return 1;
        }

        char path1[MAX_PATH_LEN];
        char path2[MAX_PATH_LEN];
        char path_out[MAX_PATH_LEN];

        st = parse_path(argv[2], path1, sizeof(path1));
        if (st != SUCCESS) { print_error(st); return st; }

        st = parse_path(argv[3], path2, sizeof(path2));
        if (st != SUCCESS) { print_error(st); return st; }

        st = parse_path(argv[4], path_out, sizeof(path_out));
        if (st != SUCCESS) { print_error(st); return st; }

        st = process_flag_r(path1, path2, path_out);

    } else if (flag == 'a') {
        if (argc != 4) {
            fprintf(stderr, "Ошибка: для флага -a требуется ровно 2 пути к файлам.\n");
            return 1;
        }

        char path_in[MAX_PATH_LEN];
        char path_out[MAX_PATH_LEN];

        st = parse_path(argv[2], path_in, sizeof(path_in));
        if (st != SUCCESS) { print_error(st); return st; }

        st = parse_path(argv[3], path_out, sizeof(path_out));
        if (st != SUCCESS) { print_error(st); return st; }

        st = process_flag_a(path_in, path_out);
    }

    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    printf("Операция успешно завершена.\n");
    return 0;
}