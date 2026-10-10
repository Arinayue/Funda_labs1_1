#ifndef FUNC_H
#define FUNC_H
#include "status.h"

status rdm_arr_mn_mx(const long a, const long b, long* nums, long size_nums);

status create_arrays_a_b_c(long** out_a, long* out_size_a, long** out_b, long* out_size_b, long** out_c);

#endif