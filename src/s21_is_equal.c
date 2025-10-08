#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;

  if (s21_is_zero(dec1) && s21_is_zero(dec2)) {
    result = 1;
  } else if (get_sign(dec1) == get_sign(dec2)) {
    s21_decimal a = dec1;
    s21_decimal b = dec2;
    int scale_a = get_scale(a);
    int scale_b = get_scale(b);
    int mismatch = 0;

    while (!mismatch && scale_a != scale_b) {
      if (scale_a > scale_b) {
        mismatch = s21_div_by_10(&a, &scale_a);
      } else {
        mismatch = s21_div_by_10(&b, &scale_b);
      }
    }

    if (!mismatch && (unsigned int)a.bits[0] == (unsigned int)b.bits[0] &&
        (unsigned int)a.bits[1] == (unsigned int)b.bits[1] &&
        (unsigned int)a.bits[2] == (unsigned int)b.bits[2]) {
      result = 1;
    }
  }

  return result;
}
