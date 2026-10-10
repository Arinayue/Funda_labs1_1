#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "parse.h"

status print_error(const status err)
{
    switch (err)
    {
    case ERROR_INVALID_ARGUMENTS:
        printf("Ошибка: Некорректное количество или значение аргументов.\n");
        break;
    case ERROR_MEMORY_ALLOCATION:
        printf("Ошибка: Некорректное выделение памяти.\n");
        break;
    case ERROR_OVERFLOW:
        printf("Ошибка: Переполнение переменной.\n");
        break;
    case ERROR_NO_RESULT:
        printf("Ошибка: Результат не существует или не определён.\n");
        break;
    default:
        printf("Неизвестная ошибка.\n");
        break;
    }
    return SUCCESS;
}

static double eq_sqrt2(const double x) { return x * x - 2.0; }
static double eq_cos(const double x) { return cos(x) - x; }
static double eq_cubic(const double x) { return x * x * x - x - 2.0; }
static double eq_sin(const double x) { return sin(x); }
static double eq_no_root(const double x) { return x * x + 1.0; }

int main(void)
{
    status st = SUCCESS;
    const double eps = 1e-9;

    printf("--- Подзадача 1 ---\n");
    int convex = 0;

    st = is_convex_polygon(&convex, eps, 4, 0.0, 0.0, 4.0, 0.0, 4.0, 3.0, 0.0, 3.0);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Прямоугольник: %s\n", convex ? "выпуклый" : "невыпуклый");

    st = is_convex_polygon(&convex, eps, 5, 0.0, 0.0, 4.0, 0.0, 2.0, 1.0, 4.0, 4.0, 0.0, 4.0);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Стрела: %s\n", convex ? "выпуклый" : "невыпуклый");

    st = is_convex_polygon(&convex, eps, 5, 0.0, 2.0, 1.1756, -1.618, -1.9021, 0.618,
                           1.9021, 0.618, -1.1756, -1.618);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Пентаграмма: %s\n", convex ? "выпуклый" : "невыпуклый");

    printf("Две вершины (ожидается ошибка):\n");
    st = is_convex_polygon(&convex, eps, 2, 0.0, 0.0, 1.0, 1.0);
    print_error(st);

    printf("Вершины на одной прямой (ожидается ошибка):\n");
    st = is_convex_polygon(&convex, eps, 3, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0);
    print_error(st);
    printf("\n");

    printf("--- Подзадача 2 ---\n");
    double value = 0.0;

    st = poly_value(&value, 2.0, 3, 2.0, -3.0, 0.0, 5.0);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("2x^3 - 3x^2 + 5 при x = 2: %g\n", value);

    st = poly_value(&value, 100.0, 0, 7.5);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Константа 7.5: %g\n", value);

    printf("Степень -1 (ожидается ошибка):\n");
    st = poly_value(&value, 1.0, -1);
    print_error(st);

    printf("x = 1e200, степень 2 (ожидается ошибка):\n");
    st = poly_value(&value, 1e200, 2, 1.0, 1.0, 1.0);
    print_error(st);
    printf("\n");

    printf("--- Подзадача 3 ---\n");
    const char *found[16];
    size_t found_count = 0;

    st = find_kaprekar(found, &found_count, 10, 9, "1", "9", "10", "45", "55", "99",
                       "100", "297", "703");
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Основание 10, найдено %zu:", found_count);
    for (size_t i = 0; i < found_count; ++i)
        printf(" %s", found[i]);
    printf("\n");

    st = find_kaprekar(found, &found_count, 16, 5, "1", "6", "A", "F", "55");
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("Основание 16, найдено %zu:", found_count);
    for (size_t i = 0; i < found_count; ++i)
        printf(" %s", found[i]);
    printf("\n");

    printf("Некорректная строка (ожидается ошибка):\n");
    st = find_kaprekar(found, &found_count, 10, 2, "45", "12x");
    print_error(st);

    printf("Основание 1 (ожидается ошибка):\n");
    st = find_kaprekar(found, &found_count, 1, 1, "1");
    print_error(st);
    printf("\n");

    printf("--- Подзадача 4 ---\n");
    double mean = 0.0;

    st = geometric_mean(&mean, eps, 3, 1.0, 3.0, 9.0);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("gm(1, 3, 9) = %g\n", mean);

    st = geometric_mean(&mean, eps, 4, 2.0, 8.0, 4.0, 16.0);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("gm(2, 8, 4, 16) = %g\n", mean);

    printf("Отрицательное число (ожидается ошибка):\n");
    st = geometric_mean(&mean, eps, 2, 5.0, -4.0);
    print_error(st);

    printf("Ноль аргументов (ожидается ошибка):\n");
    st = geometric_mean(&mean, eps, 0);
    print_error(st);
    printf("\n");

    printf("--- Подзадача 5 ---\n");
    double power = 0.0;

    st = fast_pow(&power, 2.0, 10, 1e-12);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("2^10 = %g\n", power);

    st = fast_pow(&power, 2.5, -3, 1e-12);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("2.5^(-3) = %.10g\n", power);

    st = fast_pow(&power, -3.0, 5, 1e-12);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("(-3)^5 = %g\n", power);

    printf("0^0 (ожидается ошибка):\n");
    st = fast_pow(&power, 0.0, 0, 1e-12);
    print_error(st);

    printf("0^(-2) (ожидается ошибка):\n");
    st = fast_pow(&power, 0.0, -2, 1e-12);
    print_error(st);

    printf("10^1000 (ожидается ошибка):\n");
    st = fast_pow(&power, 10.0, 1000, 1e-12);
    print_error(st);
    printf("\n");

    printf("--- Подзадача 6 ---\n");
    double root = 0.0;
    const double eps_list[] = {1e-3, 1e-6, 1e-12};

    for (size_t i = 0; i < sizeof eps_list / sizeof eps_list[0]; ++i)
    {
        st = bisect_root(&root, 0.0, 2.0, eps_list[i], eq_sqrt2);
        if (st != SUCCESS)
        {
            print_error(st);
            return st;
        }
        printf("x^2 - 2 = 0 на [0, 2], eps = %g: x = %.12f\n", eps_list[i], root);
    }

    st = bisect_root(&root, 0.0, 1.0, 1e-9, eq_cos);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("cos(x) - x = 0 на [0, 1], eps = 1e-9: x = %.10f\n", root);

    st = bisect_root(&root, 1.0, 2.0, 1e-9, eq_cubic);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("x^3 - x - 2 = 0 на [1, 2], eps = 1e-9: x = %.10f\n", root);

    st = bisect_root(&root, 3.0, 4.0, 1e-9, eq_sin);
    if (st != SUCCESS)
    {
        print_error(st);
        return st;
    }
    printf("sin(x) = 0 на [3, 4], eps = 1e-9: x = %.10f\n", root);

    printf("x^2 + 1 на [-1, 1] (ожидается ошибка):\n");
    st = bisect_root(&root, -1.0, 1.0, 1e-9, eq_no_root);
    print_error(st);

    printf("Перепутаны границы (ожидается ошибка):\n");
    st = bisect_root(&root, 2.0, 0.0, 1e-9, eq_sqrt2);
    print_error(st);

    printf("eps = 0 (ожидается ошибка):\n");
    st = bisect_root(&root, 0.0, 2.0, 0.0, eq_sqrt2);
    print_error(st);

    printf("f = NULL (ожидается ошибка):\n");
    st = bisect_root(&root, 0.0, 2.0, 1e-9, NULL);
    print_error(st);

    return SUCCESS;
}