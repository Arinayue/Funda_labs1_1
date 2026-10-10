#include <stdio.h>
#include "status.h"
#include "parse.h"
#include "functions.h"

static void print_error(const status_code err) {
    switch (err) {
        case STATUS_INVALID_INPUT:
            printf("Ошибка: Некорректный аргумент. Ожидается число epsilon в диапазоне (0, 1).\n");
            break;
        case STATUS_OVERFLOW:
            printf("Ошибка: Переполнение при считывании или вычислении.\n");
            break;
        case STATUS_MAX_ITERATIONS:
            printf("Ошибка: Превышено максимальное число итераций (метод не сошелся).\n");
            break;
        default:
            printf("Неизвестная ошибка (код %d).\n", err);
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <eps>\n", argv[0]);
        return STATUS_INVALID_INPUT;
    }

    double eps;
    status_code err = parse_eps(argv[1], &eps);
    if (err != STATUS_OK) {
        print_error(err);
        return err;
    }

    const ConstantMethod methods[] = {
        {"e",     "Limit",    e_limit},
        {"e",     "Series",   e_series},
        {"e",     "Equation", e_equation},

        {"pi",    "Limit",    pi_limit},
        {"pi",    "Series",   pi_series},
        {"pi",    "Equation", pi_equation},

        {"ln2",   "Limit",    ln2_limit},
        {"ln2",   "Series",   ln2_series},
        {"ln2",   "Equation", ln2_equation},

        {"sqrt2", "Limit",    sqrt2_limit},
        {"sqrt2", "Series",   sqrt2_series},
        {"sqrt2", "Equation", sqrt2_equation},

        {"gamma", "Limit",    gamma_limit},
        {"gamma", "Series",   gamma_series},
        {"gamma", "Equation", gamma_equation}
    };
    const int methods_count = sizeof(methods) / sizeof(methods[0]);

    printf("  Constant | Method    | Result          \n");

    for (int i = 0; i < methods_count; i++) {
        double val = 0.0;
        status_code st = methods[i].func(eps, &val);

        if (st == STATUS_OK) {
            printf("  %-8s | %-9s | %.10f\n", 
                   methods[i].const_name, 
                   methods[i].method_name, 
                   val);
        } else {
            fprintf(stderr, "[Предупреждение] %s (%s) завершился с ошибкой %d\n", 
                    methods[i].const_name, methods[i].method_name, st);
            
            printf("  %-8s | %-9s | ERROR\n", 
                   methods[i].const_name, 
                   methods[i].method_name);
        }
    }


    return STATUS_OK;
}