#include <stdio.h>
#include "status.h"
#include "parse.h"
#include "series.h"
#include "integration.h"

void print_error(const status err) {
    switch (err) {
        case ERROR_INVALID_ARGUMENTS:
            printf( "Ошибка: Некорректное количество или значение аргументов.\n");
            break;
        case ERROR_CONVERGENCE_FAILED:
            printf("Ошибка: Превышено максимальное число итераций, ряд не сходится.\n");
            break;
        case ERROR_OVERFLOW:
            printf( "Ошибка: Произошло переполнение при вычислениях.\n");
            break;
        case ERROR_ZERO_DIVISION:
            printf("Ошибка: Деление на ноль строго запрещено.\n");
            break;
        default:
            printf("Ошибка: Неизвестный код ошибки (%d).\n", err);
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование:\n");
        fprintf(stderr, "  %s <eps> <x>\n", argv[0]);
        return ERROR_INVALID_ARGUMENTS;
    }

    double eps = 0.0;
    double x = 0.0;
    status err;

    if ((err = parse_double(argv[1], &eps)) != SUCCESS) {
        print_error(err);
        return err;
    }

    if ((err = parse_double(argv[2], &x)) != SUCCESS) {
        print_error(err);
        return err;
    }

    if (eps <= 0.0) {
        print_error(ERROR_INVALID_ARGUMENTS);
        return ERROR_INVALID_ARGUMENTS;
    }

    printf("--- Ряды ---\n");
    double res = 0.0;

    if (sum_a(x, eps, &res) == SUCCESS) printf("  a: %.10lf\n", res);
    else printf("  a: Ошибка вычисления\n");

    if (sum_b(x, eps, &res) == SUCCESS) printf("  b: %.10lf\n", res);
    else printf("  b: Ошибка вычисления\n");

    if (sum_c(x, eps, &res) == SUCCESS) printf("  c: %.10lf\n", res);
    else printf("  c: Ошибка вычисления (|x| должен быть < 1)\n");

    if (sum_d(x, eps, &res) == SUCCESS) printf("  d: %.10lf\n", res);
    else printf("  d: Ошибка вычисления (|x| должен быть < 1)\n");

    printf("\n--- Интегралы ---\n");

    if (integrate(func_a, 0.0, 1.0, eps, &res) == SUCCESS) printf("  a: %.10lf\n", res);
    else printf("  a: Ошибка интегрирования\n");

    if (integrate(func_b, 0.0, 1.0, eps, &res) == SUCCESS) printf("  b: %.10lf\n", res);
    else printf("  b: Ошибка интегрирования\n");

    if (integrate(func_c, 0.0, 1.0 - eps, eps, &res) == SUCCESS) printf("  c: %.10lf\n", res);
    else printf("  c: Ошибка интегрирования\n");

    if (integrate(func_d, 0.0, 1.0, eps, &res) == SUCCESS) printf("  d: %.10lf\n", res);
    else printf("  d: Ошибка интегрирования\n");

    return SUCCESS;
}