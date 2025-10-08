#include <string.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(s21_decimal));

  if (!s21_is_decimal(value_1) || !s21_is_decimal(value_2) || result == NULL) {
    if (result) s21_set_zero(result);
    return NUMBER_IS_TOO_BIG;
  }

  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  s21_decimal a = value_1;
  s21_decimal b = value_2;

  if (s21_align_scale(&a, &b) != 0) {
    return NUMBER_IS_TOO_BIG;
  }

  int result_scale = get_scale(a);

  if (sign_1 == sign_2) {
    if (add_mantissa(&a, &b, result) != 0) {
      if (banker_round(&a) != 0 || banker_round(&b) != 0) {
        return (sign_1 == 0) ? NUMBER_IS_TOO_BIG : NUMBER_IS_TOO_SMALL;
      }
      result_scale = get_scale(a);
      if (add_mantissa(&a, &b, result) != 0) {
        return (sign_1 == 0) ? NUMBER_IS_TOO_BIG : NUMBER_IS_TOO_SMALL;
      }
    }
    set_sign(result, sign_1);
  } else {
    int cmp = compare_mantissa(&a, &b);

    if (cmp == 0) {
      memset(result, 0, sizeof(s21_decimal));
      set_scale(result, result_scale);
      return OK;
    } else if (cmp > 0) {
      subtract_mantissa(&a, &b, result);
      set_sign(result, sign_1);
    } else {
      subtract_mantissa(&b, &a, result);
      set_sign(result, sign_2);
    }
  }

  set_scale(result, result_scale);

  // normalize_result(result);

  return OK;
}