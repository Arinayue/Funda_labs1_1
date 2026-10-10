#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "parse.h"
#include "status.h"
#include "func.h"

#define SIZE_BUF 256

void print_error(const status err) {
    switch (err) {
        case ERROR_INVALID_ARGUMENTS:
            printf("Ошибка: Некорректное количество или значение аргументов.\n");
            break;
        case ERROR_OVERFLOW:
            printf("Ошибка: Переполнение переменной.\n");
            break;
        default:
            printf("Ошибка: Неизвестный код ошибки (%d).\n", err);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_error(ERROR_INVALID_ARGUMENTS);
        return ERROR_INVALID_ARGUMENTS;
    }

    char str[64];
    long base;
    long long sum = 0;
    long long max = 0;
    int count = 0;

    status err = parse_base(argv[1], &base);
    if (err != SUCCESS) {
        print_error(err);
        return err;
    }

    while (1) {
        /* EOF или ошибка ввода до "Stop" */
        if (scanf("%63s", str) != 1) {
            print_error(ERROR_INVALID_ARGUMENTS);
            return ERROR_INVALID_ARGUMENTS;
        }

        if (strcmp(str, "Stop") == 0) {
            break;
        }

        long long num;
        err = str_to_num(str, base, &num);
        if (err != SUCCESS) {
            print_error(err);
            return err;
        }

        /* проверка переполнения суммы */
        if (num > 0 && sum > 0) {
            if (num > LLONG_MAX - sum) {
                print_error(ERROR_OVERFLOW);
                return ERROR_OVERFLOW;
            }
        } else if (num < 0 && sum < 0) {
            if (num < LLONG_MIN - sum) {
                print_error(ERROR_OVERFLOW);
                return ERROR_OVERFLOW;
            }
        }
        sum += num;

        /* максимум по модулю: сравниваем через отрицательные значения,
           чтобы избежать UB при LLONG_MIN */
        if (count == 0) {
            max = num;
        } else {
            long long neg_num = (num > 0) ? -num : num;
            long long neg_max = (max > 0) ? -max : max;
            if (neg_num < neg_max) {
                max = num;
            }
        }

        count++;
    }

    /* ни одного числа не введено */
    if (count == 0) {
        print_error(ERROR_INVALID_ARGUMENTS);
        return ERROR_INVALID_ARGUMENTS;
    }

    /* сами числа в десятичной системе */
    printf("%lld %lld\n", sum, max);

    char buf[SIZE_BUF];
    for (int b = 9; b <= 36; b += 9) {
        err = num_to_str(sum, b, buf, SIZE_BUF);
        if (err != SUCCESS) {
            print_error(err);
            return err;
        }
        printf("%s ", buf);

        err = num_to_str(max, b, buf, SIZE_BUF);
        if (err != SUCCESS) {
            print_error(err);
            return err;
        }
        printf("%s\n", buf);
    }

    return 0;
}