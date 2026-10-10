#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "status.h"
#include "func.h"
#include "parse.h"

status print_error(const status err) {
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
    default:
        printf("Неизвестная ошибка.\n");
        break;
    }
    return SUCCESS;
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        print_error(ERROR_INVALID_ARGUMENTS);
        return ERROR_INVALID_ARGUMENTS;
    }

    long a = 0, b = 0, size_nums = 0;
    status st = parse_long(argv[1], &a);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    st = parse_long(argv[2], &b);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    st = parse_long(argv[3], &size_nums);
    if (st != SUCCESS || size_nums <= 0) {
        print_error(ERROR_INVALID_ARGUMENTS);
        return ERROR_INVALID_ARGUMENTS;
    }

    srand((unsigned int)time(NULL));

    printf("--- Подзадача 9.1 ---\n");
    long* nums = (long*)malloc(sizeof(long) * size_nums);
    if (nums == NULL) {
        print_error(ERROR_MEMORY_ALLOCATION);
        return ERROR_MEMORY_ALLOCATION;
    }

    st = rdm_arr_mn_mx(a, b, nums, size_nums);
    if (st != SUCCESS) {
        free(nums);
        print_error(st);
        return st;
    }

    printf("Массив после генерации и обмена min/max:\n");
    for (long i = 0; i < size_nums; i++) {
        printf("%ld ", nums[i]);
    }
    printf("\n\n");

    free(nums);

    printf("--- Подзадача 9.2 ---\n");
    long *arr_a = NULL, *arr_b = NULL, *arr_c = NULL;
    long size_a = 0, size_b = 0;

    st = create_arrays_a_b_c(&arr_a, &size_a, &arr_b, &size_b, &arr_c);
    if (st != SUCCESS) {
        print_error(st);
        return st;
    }

    printf("Размер A: %ld, Размер B: %ld\n", size_a, size_b);
    

    for (long i = 0; i < size_a; i++) {
        printf("%ld ", arr_a[i]);
    }
    printf("\n");

    for (long i = 0; i < size_b; i++) {
        printf("%ld ", arr_b[i]);
    }
    printf("\n");

    for (long i = 0; i < size_a; i++) {
        printf("%ld ", arr_c[i]);
    }
    printf("\n");

    free(arr_a);
    free(arr_b);
    free(arr_c);

    return SUCCESS;
}