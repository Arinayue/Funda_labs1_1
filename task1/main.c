#include "status.h"
#include "parsing.h"
#include "handlers.h"
#include <stdio.h>

void print_error(const status_code err) {
    switch (err) {
        case STATUS_INVALID_INPUT:
            printf("Ошибка: Некорректный ввод аргументов.\n");
            break;
        case STATUS_INVALID_RANGE:
            printf("Ошибка: Число выходит за допустимый диапазон для данного флага.\n");
            break;
        case STATUS_OVERFLOW:
            printf("Ошибка: Произошло переполнение типом данных.\n");
            break;
        case STATUS_MEMORY_ERROR:
            printf("Ошибка: Не удалось выделить память.\n");
            break;
        case STATUS_NOT_FOUND:
            printf("Ошибка: Результат не найден.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_error(STATUS_INVALID_INPUT);
        return STATUS_INVALID_INPUT;
    }

    long x;
    char flag;
    long buffer[128];
    int count = 0;

    FlagMap handlers[] = {
        {'h', process_h},
        {'p', process_p},
        {'s', process_s},
        {'e', process_e},
        {'a', process_a},
        {'f', process_f}
    };
    const int HANDLERS_COUNT = sizeof(handlers) / sizeof(handlers[0]);

    status_code err = parse_num(argv[1], &x);
    if (err != STATUS_OK) {
        print_error(err);
        return err;
    }

    err = parse_flag(argv[2], &flag);
    if (err != STATUS_OK) {
        print_error(err);
        return err;
    }

    for (int i = 0; i < HANDLERS_COUNT; i++) {
        if (handlers[i].flag == flag) {
            status_code status = handlers[i].handler(x, buffer, &count);
            if (status != STATUS_OK) {
                print_error(status);
                return status;
            }

            if (flag == 's') {
                for (int j = 0; j < count; j++) {
                    printf("%c", (char)buffer[j]);
                }
            } else {
                for (int j = 0; j < count; j++) {
                    printf("%ld ", buffer[j]);
                }
            }
            printf("\n");
            return STATUS_OK; 
        }
    }

    print_error(STATUS_INVALID_INPUT);
    return STATUS_INVALID_INPUT;
}