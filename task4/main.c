#include <stdio.h>
#include <string.h>
#include "status.h"
#include "parse.h"
#include "func.h"

#define PATH_BUF_SIZE 512

void print_error(const status err) {
    switch (err) {
        case SUCCESS:
            break;
        case ERROR_INVALID_PARAMETER:
            printf("Ошибка: Некорректный параметр, флаг или путь к файлу.\n");
            break;
        case ERROR_FILE_OPEN:
            printf("Ошибка: Не удалось открыть файл.\n");
            break;
        case ERROR_SAME_FILE:
            printf("Ошибка: Входной и выходной файлы совпадают.\n");
            break;
        case ERROR_FILE_WRITE:
            printf("Ошибка: Не удалось записать в выходной файл.\n");
            break;
        case ERROR_FILE_READ:
            printf("Ошибка: Не удалось прочитать входной файл.\n");
            break;            
        default:
            printf("Ошибка: Произошел неизвестный сбой (%d).\n", err);
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Использование: %s <флаг> <входной_файл> [выходной_файл]\n", argv[0]);
        print_error(ERROR_INVALID_PARAMETER);
        return ERROR_INVALID_PARAMETER;
    }

    char flag[4];
    status st = parse_flag(argv[1], flag);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    char in_path[PATH_BUF_SIZE];
    st = parse_path(argv[2], in_path, PATH_BUF_SIZE);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    int has_n = (strlen(flag) == 3);

    if ((has_n && argc != 4) || (!has_n && argc != 3)) {
        print_error(ERROR_INVALID_PARAMETER);
        return ERROR_INVALID_PARAMETER;
    }

    char out_path[PATH_BUF_SIZE];

    if (has_n) {
        st = parse_path(argv[3], out_path, PATH_BUF_SIZE);
    } else {
        st = generate_output_path(in_path, out_path, PATH_BUF_SIZE);
    }
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    st = check_same_file(in_path, out_path);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    char action = has_n ? flag[2] : flag[1];

    switch (action) {
        case 'd':
            st = func_d(in_path, out_path);
            break;
        case 'i':
            st = func_i(in_path, out_path);
            break;
        case 's':
            st = func_s(in_path, out_path);
            break;
        case 'a':
            st = func_a(in_path, out_path);
            break;
        default:
            st = ERROR_INVALID_PARAMETER;
            break;
    }

    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    printf("Успешно обработано! Результат записан в: %s\n", out_path);
    return SUCCESS;
}