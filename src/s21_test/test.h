#ifndef TEST_H
#define TEST_H
#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"

#define TOTAL_BYTES 4
#define MAX_BIT 32
#define TOTAL_BITS 96
#define MAX_EXP 28
#define FLOAT_ACCURACY 7

#define MINUS 0x80000000
#define EXP 0x00ff0000
#define BROKEN_MASK 0x7FE0FFFF

#define SET_ZERO(val) ((val) = (s21_decimal){0})
#define SET_MINUS(val) ((val)->bits[3] |= MINUS)
#define CHECK_MINUS(val) ((val.bits[3] & MINUS) ? 0 : 1)
#define verification_zero(val) \
  ((val).bits[0] == 0 && (val).bits[1] == 0 && (val).bits[2] == 0)
#define check_broken(val) (val.bits[3] & BROKEN_MASK)

#define MAX_DEC 79228162514264337593543950335.0f - 1.0
#define MIN_DEC -79228162514264337593543950335.0f + 1.0

Suite *test_add(void);
Suite *test_sub(void);
Suite *test_mul(void);
Suite *test_div(void);

Suite *test_is_less(void);
Suite *test_is_less_or_equal(void);
Suite *test_is_greater(void);
Suite *test_is_greater_or_equal(void);
Suite *test_is_equal(void);
Suite *test_is_not_equal(void);

Suite *test_from_int_to_decimal(void);
Suite *test_from_float_to_decimal(void);
Suite *test_from_decimal_to_int(void);
Suite *test_from_decimal_to_float(void);

Suite *test_floor(void);
Suite *test_round(void);
Suite *test_truncate(void);
Suite *test_negate(void);
#endif
