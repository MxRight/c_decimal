#include <limits.h>
#include <stddef.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) return CONVERTATION_ERROR;

  s21_set_zero(dst);

  if (src == 0) return OK;

  if (src < 0) {
    if (src == INT_MIN) {
      dst->bits[0] = 0x80000000;
    } else {
      dst->bits[0] = -src;
    }
    set_sign(dst, 1);
  } else {
    dst->bits[0] = src;
  }
  return OK;
}