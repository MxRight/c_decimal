#include <check.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "../s21_helper_functions.h"
#include "test.h"

START_TEST(test_less_positive) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_negative) {
  s21_decimal a = {{2, 0, 0, 0x80000000}};
  s21_decimal b = {{1, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_diff_signs) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{1, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_less(b, a), 1);
}
END_TEST

START_TEST(test_less_equal) {
  s21_decimal a = {{123, 0, 0, 0x00000000}};
  s21_decimal b = {{123, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_with_scale) {
  s21_decimal a = {{99, 0, 0, 0x00020000}};
  s21_decimal b = {{100, 0, 0, 0x00020000}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

Suite *test_is_less(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("=s21_is_less=");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_less_positive);
  tcase_add_test(tc_core, test_less_negative);
  tcase_add_test(tc_core, test_less_diff_signs);
  tcase_add_test(tc_core, test_less_equal);
  tcase_add_test(tc_core, test_less_with_scale);
  tcase_add_test(tc_core, test_less_zero);

  suite_add_tcase(s, tc_core);
  return s;
}