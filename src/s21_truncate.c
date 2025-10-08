#include <stdio.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!s21_is_decimal(value) || result == NULL) {
    return 1;
  }

  //*result = value;
  int scale = get_scale(*result);

  if (scale == 0) {
    return 0;
  }

  set_scale(result, 0);

  int remainder = 0;
  for (int i = 0; i < scale; i++) {
    s21_div_by_10(result, &remainder);
  }

  return 0;
}