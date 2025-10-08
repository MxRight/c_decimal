#include <math.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return CONVERTATION_ERROR;

  *dst = 0.0f;
  double temp = 0.0;
  int sign = get_sign(src);
  int scale = get_scale(src);

  temp = (double)(unsigned int)src.bits[0];
  temp += (double)(unsigned int)src.bits[1] * 4294967296.0;
  temp += (double)(unsigned int)src.bits[2] * 18446744073709551616.0;

  if (scale > 0) {
    double divisor = 1.0;
    for (int i = 0; i < scale; i++) {
      divisor *= 10.0;
    }
    temp /= divisor;
  }

  *dst = (float)temp;
  if (sign) *dst = -(*dst);

  if (fabs(*dst) < 1e-28 && fabs(*dst) > 0) {
    *dst = 0.0f;
  }

  return OK;
}