#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  int result = 0;

  if (s21_is_zero(a) && s21_is_zero(b)) {
    result = 1;
  } else {
    int signA = get_sign(a);
    int signB = get_sign(b);
    int scaleA = get_scale(a);
    int scaleB = get_scale(b);
    int max_scale = (scaleA > scaleB) ? scaleA : scaleB;
    int diffA = max_scale - scaleA;
    int diffB = max_scale - scaleB;
    s21_decimal a_norm = a;
    s21_decimal b_norm = b;
    int overflowA = diffA > 0 ? s21_mul_10_exponent(&a_norm, diffA) : 0;
    int overflowB = diffB > 0 ? s21_mul_10_exponent(&b_norm, diffB) : 0;

    if (signA != signB) {
      result = signA;
    } else if (overflowA || overflowB) {
      result = signA ? overflowA : !overflowA;
    } else {
      int cmp = s21_compare_bits(a_norm, b_norm);
      result = signA ? (cmp >= 0) : (cmp <= 0);
    }
  }

  return result;
}