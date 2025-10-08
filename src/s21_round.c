#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) return CALCULATION_ERROR;
  s21_set_zero(result);

  const int sign = get_sign(value);
  const int scale = get_scale(value);
  if (scale == 0) {
    *result = value;
    return OK;
  }

  s21_decimal abs_value = value;
  set_sign(&abs_value, 0);

  s21_decimal quotient = abs_value;
  int remainder = 0;
  int dummy_scale = scale;

  for (int i = 0; i < scale; i++) {
    remainder = s21_div_by_10(&quotient, &dummy_scale);
  }

  if (remainder > 5 || (remainder == 5 && (quotient.bits[0] & 1))) {
    unsigned int carry = 1;
    for (int i = 0; i < 3 && carry; i++) {
      unsigned long long sum = (unsigned long long)quotient.bits[i] + carry;
      quotient.bits[i] = (unsigned int)(sum & 0xFFFFFFFF);
      carry = (unsigned int)(sum >> 32);
    }
    if (carry) {
      return sign ? NUMBER_IS_TOO_SMALL : NUMBER_IS_TOO_BIG;
    }
  }

  *result = quotient;
  if (sign) set_sign(result, 1);
  set_scale(result, 0);

  return OK;
}