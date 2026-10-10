#include <stdio.h>
#include "status.h"
#include "parse.h"
#include "func.h"

void print_error(const status err) {
    switch (err) {
        case ERROR_INVALID_ARGUMENTS:
            printf("Ошибка: Некорректное количество или значение аргументов.\n");
            break;
        case ERROR_UNKNOWN_FLAG:
            printf("Ошибка: Неизвестный флаг. Поддерживаются: -q, -m, -t (или /q, /m, /t).\n");
            break;
        case ERROR_INVALID_NUMBER:
            printf("Ошибка: Некорректный формат числа.\n");
            break;
        case ERROR_ZERO_DIVISION:
            printf("Ошибка: Деление на ноль строго запрещено.\n");
            break;
        case ERROR_INVALID_DATA:
            printf("Ошибка: Входные данные не соответствуют предметной области.\n");
            break;
        default:
            printf("Ошибка: Неизвестный код ошибки (%d).\n", err);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование:\n");
        fprintf(stderr, "  %s -q <eps> <a> <b> <c>\n", argv[0]);
        fprintf(stderr, "  %s -m <a> <b>\n", argv[0]);
        fprintf(stderr, "  %s -t <eps> <a> <b> <c>\n", argv[0]);
        return ERROR_INVALID_ARGUMENTS;
    }

    char flag;
    status err = parse_flag(argv[1], &flag);
    if (err != SUCCESS) {
        print_error(err);
        return err;
    }

    switch (flag) {
        case 'q': {
            if (argc != 6) {
                fprintf(stderr, "Ошибка: Флаг -q требует 4 параметра: <eps> <a> <b> <c>\n");
                return ERROR_INVALID_ARGUMENTS;
            }
            double eps, a, b, c;

            if ((err = parse_double(argv[2], &eps)) != SUCCESS) { print_error(err); return err; }
            if ((err = parse_double(argv[3], &a)) != SUCCESS)   { print_error(err); return err; }
            if ((err = parse_double(argv[4], &b)) != SUCCESS)   { print_error(err); return err; }
            if ((err = parse_double(argv[5], &c)) != SUCCESS)   { print_error(err); return err; }

            Solution solutions[6];
            int unique_count = 0;

            err = func_q(eps, a, b, c, solutions, &unique_count);
            if (err != SUCCESS) {
                print_error(err);
                return err;
            }

            for (int i = 0; i < unique_count; i++) {
                printf("Уравнение: (%.4lf)*x^2 + (%.4lf)*x + (%.4lf) = 0\n", 
                       solutions[i].a, solutions[i].b, solutions[i].c);
                switch (solutions[i].count) {
                    case -1:
                        printf("  -> Бесконечно много решений\n");
                        break;
                    case 0:
                        printf("  -> Нет решений\n");
                        break;
                    case 1:
                        printf("  -> Один корень: x = %lf\n", solutions[i].x1);
                        break;
                    case 2:
                        printf("  -> Два корня: x1 = %lf, x2 = %lf\n", solutions[i].x1, solutions[i].x2);
                        break;
                }
            }
            break;
        }

        case 'm': {
            if (argc != 4) {
                fprintf(stderr, "Ошибка: Флаг -m требует 2 параметра: <a> <b>\n");
                return ERROR_INVALID_ARGUMENTS;
            }
            long a, b;

            if ((err = parse_long(argv[2], &a)) != SUCCESS) { print_error(err); return err; }
            if ((err = parse_long(argv[3], &b)) != SUCCESS) { print_error(err); return err; }

            int is_multiple = 0;
            err = func_m(a, b, &is_multiple);
            if (err != SUCCESS) {
                print_error(err);
                return err;
            }

            if (is_multiple) {
                printf("Число %ld кратно %ld\n", a, b);
            } else {
                printf("Число %ld НЕ кратно %ld\n", a, b);
            }
            break;
        }

        case 't': {
            if (argc != 6) {
                fprintf(stderr, "Ошибка: Флаг -t требует 4 параметра: <eps> <a> <b> <c>\n");
                return ERROR_INVALID_ARGUMENTS;
            }
            double eps, a, b, c;

            if ((err = parse_double(argv[2], &eps)) != SUCCESS) { print_error(err); return err; }
            if ((err = parse_double(argv[3], &a)) != SUCCESS)   { print_error(err); return err; }
            if ((err = parse_double(argv[4], &b)) != SUCCESS)   { print_error(err); return err; }
            if ((err = parse_double(argv[5], &c)) != SUCCESS)   { print_error(err); return err; }

            int is_right = 0;
            err = func_t(eps, a, b, c, &is_right);
            if (err != SUCCESS) {
                print_error(err);
                return err;
            }

            if (is_right) {
                printf("Стороны %.4lf, %.4lf, %.4lf образуют прямоугольный треугольник\n", a, b, c);
            } else {
                printf("Стороны %.4lf, %.4lf, %.4lf НЕ образуют прямоугольный треугольник\n", a, b, c);
            }
            break;
        }

        default:
            print_error(ERROR_UNKNOWN_FLAG);
            return ERROR_UNKNOWN_FLAG;
    }

    return SUCCESS;
}