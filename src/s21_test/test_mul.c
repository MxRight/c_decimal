#include <check.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "../s21_helper_functions.h"
#include "test.h"

START_TEST(test_mul_ok) {
  s21_decimal a = {{2, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 6);
  ck_assert_uint_eq(get_scale(res), 0);
  ck_assert_uint_eq(get_sign(res), 0);
}
END_TEST

START_TEST(test_mul_zero_left) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{123, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 0);
}
END_TEST

START_TEST(test_mul_zero_right) {
  s21_decimal a = {{123, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 0);
}
END_TEST

START_TEST(test_mul_one) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{123, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 123);
}
END_TEST

START_TEST(test_mul_sign_pos_neg) {
  s21_decimal a = {{2, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x80000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 6);
  ck_assert_uint_eq(get_sign(res), 1);
}
END_TEST

START_TEST(test_mul_sign_neg_neg) {
  s21_decimal a = {{2, 0, 0, 0x80000000}};
  s21_decimal b = {{3, 0, 0, 0x80000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 6);
  ck_assert_uint_eq(get_sign(res), 0);
}
END_TEST

START_TEST(test_mul_scale) {
  s21_decimal a = {{12, 0, 0, 0x00010000}};
  s21_decimal b = {{15, 0, 0, 0x00010000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_eq(res.bits[0], 180);
  ck_assert_uint_eq(get_scale(res), 2);
}
END_TEST

START_TEST(test_mul_scale_reduce) {
  s21_decimal a = {{1, 0, 0, 0x001C0000}};
  s21_decimal b = {{1, 0, 0, 0x001C0000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_uint_le(get_scale(res), 28);
}
END_TEST

START_TEST(test_mul_too_big) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NUMBER_IS_TOO_BIG);
}
END_TEST

START_TEST(test_mul_too_small) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0x80000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NUMBER_IS_TOO_SMALL);
}
END_TEST

START_TEST(test_mul_invalid_decimal) {
  s21_decimal a = {{1, 0, 0, 0x00200000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NUMBER_IS_TOO_BIG);
}
END_TEST

START_TEST(test_mul_null_result) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  int status = s21_mul(a, b, NULL);
  ck_assert_int_eq(status, NUMBER_IS_TOO_BIG);
}
END_TEST

START_TEST(test_mul_div_by_zero_code) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);
  ck_assert_int_ne(status, DIVISION_BY_ZERO);
}
END_TEST

START_TEST(test_mul_bankers_rounding) {
  s21_decimal a = {{15, 0, 0, 0x001D0000}};
  s21_decimal b = {{10, 0, 0, 0x001D0000}};
  s21_decimal res = {0};
  int status = s21_mul(a, b, &res);

  ck_assert_int_eq(status, NUMBER_IS_TOO_BIG);
}
END_TEST

START_TEST(test_mul_banker_round_overflow) {
  s21_decimal value_1 = {{0x99999999, 0x99999999, 0x19999999, 0x00010000}};
  s21_decimal value_2 = {{5, 0, 0, 0}};
  s21_decimal result = {0};

  int ret = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(ret, NUMBER_IS_TOO_BIG);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_mul_rounding_overflow_96bit) {
  s21_decimal value_1 = {{0x9999999A, 0x99999999, 0x19999999, 0x00010000}};
  s21_decimal value_2 = {{5, 0, 0, 0}};
  s21_decimal result = {0};

  int ret = s21_mul(value_1, value_2, &result);

  if (ret == 0) {
    s21_decimal max_with_scale = {
        {0xFFFFFFF5, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}};
    s21_decimal one = {{10, 0, 0, 0x00010000}};
    ret = s21_mul(max_with_scale, one, &result);
  }

  ck_assert_int_eq(ret, NUMBER_IS_TOO_BIG);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_mul_rounding_overflow_precise) {
  s21_decimal result = {0};

  s21_decimal a = {{0x33333333, 0x33333333, 0x33333333, 0x00010000}};
  s21_decimal b = {{5, 0, 0, 0}};

  int ret = s21_mul(a, b, &result);

  if (ret != NUMBER_IS_TOO_BIG) {
    s21_decimal max_minus_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal small_mult = {{1, 0, 0, 0x001C0000}};
    ret = s21_mul(max_minus_1, small_mult, &result);
  }

  ck_assert_int_ne(ret, OK);
}
END_TEST

Suite *test_mul(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("=s21_mul=");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mul_ok);
  tcase_add_test(tc_core, test_mul_zero_left);
  tcase_add_test(tc_core, test_mul_zero_right);
  tcase_add_test(tc_core, test_mul_one);
  tcase_add_test(tc_core, test_mul_sign_pos_neg);
  tcase_add_test(tc_core, test_mul_sign_neg_neg);
  tcase_add_test(tc_core, test_mul_scale);
  tcase_add_test(tc_core, test_mul_scale_reduce);
  tcase_add_test(tc_core, test_mul_too_big);
  tcase_add_test(tc_core, test_mul_too_small);
  tcase_add_test(tc_core, test_mul_invalid_decimal);
  tcase_add_test(tc_core, test_mul_null_result);
  tcase_add_test(tc_core, test_mul_div_by_zero_code);
  tcase_add_test(tc_core, test_mul_bankers_rounding);
  tcase_add_test(tc_core, test_mul_banker_round_overflow);
  tcase_add_test(tc_core, test_mul_rounding_overflow_96bit);
  tcase_add_test(tc_core, test_mul_rounding_overflow_precise);

  suite_add_tcase(s, tc_core);
  return s;
}