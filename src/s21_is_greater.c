#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_is_greater(s21_decimal first, s21_decimal second) {
  return !s21_is_less(first, second) && !s21_is_equal(first, second);
}