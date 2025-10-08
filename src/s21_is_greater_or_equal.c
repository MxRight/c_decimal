#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  int result = 0;
  int both_zero = s21_is_zero(a) && s21_is_zero(b);

  if (both_zero) {
    result = 1;
  } else if (get_sign(a) != get_sign(b)) {
    result = (get_sign(a) == 0);
  } else {
    int signA = get_sign(a);
    int scaleA = get_scale(a);
    int scaleB = get_scale(b);
    int max_scale = (scaleA > scaleB) ? scaleA : scaleB;
    int diffA = max_scale - scaleA;
    int diffB = max_scale - scaleB;
    s21_decimal a_norm = a;
    s21_decimal b_norm = b;
    int overflowA = 0;
    int overflowB = 0;

    if (diffA > 0) overflowA = s21_mul_10_exponent(&a_norm, diffA);
    if (diffB > 0) overflowB = s21_mul_10_exponent(&b_norm, diffB);

    if (overflowA) {
      result = (signA == 0) ? 1 : 0;
    } else if (overflowB) {
      result = (signA == 0) ? 0 : 1;
    } else {
      int cmp = s21_compare_bits(a_norm, b_norm);
      result = (signA == 0) ? (cmp >= 0) : (cmp <= 0);
    }
  }

  return result;
}