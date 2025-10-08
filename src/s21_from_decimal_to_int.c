#include <stdio.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = OK;

  if (dst == NULL || !s21_is_decimal(src)) {
    status = CONVERTATION_ERROR;
  } else {
    s21_decimal tmp = src;
    int scale = get_scale(tmp);
    while (scale > 0) {
      s21_div_by_10(&tmp, &scale);
    }
    if (tmp.bits[1] != 0 || tmp.bits[2] != 0) {
      status = CONVERTATION_ERROR;
    } else {
      int sign = get_sign(tmp);
      unsigned int value = tmp.bits[0];
      if ((!sign && value > 0x7FFFFFFF) || (sign && value > 0x80000000)) {
        status = CONVERTATION_ERROR;
      } else {
        *dst = sign ? -(int)value : (int)value;
      }
    }
  }

  return status;
}