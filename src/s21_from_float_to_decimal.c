#include <float.h>
#include <math.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = OK;

  if (!dst) {
    status = CONVERTATION_ERROR;
  } else {
    s21_set_zero(dst);

    if (isnan(src) || isinf(src)) {
      status = CONVERTATION_ERROR;
    } else {
      if (fabsf(src) < 1e-28f) {
        s21_set_zero(dst);
      } else {
        int sign = 0;
        if (src < 0) {
          sign = 1;
          src = -src;
        }

        int scale = 0;
        double value = src;

        while (scale < 28 && floor(value) != value) {
          value *= 10.0;
          scale++;
        }

        value = round(value);

        if (value > 79228162514264337593543950335.0) {
          status = CONVERTATION_ERROR;
        } else {
          unsigned int low = (unsigned int)fmod(value, 4294967296.0);
          value = floor(value / 4294967296.0);
          unsigned int mid = (unsigned int)fmod(value, 4294967296.0);
          value = floor(value / 4294967296.0);
          unsigned int high = (unsigned int)value;

          dst->bits[0] = low;
          dst->bits[1] = mid;
          dst->bits[2] = high;
          dst->bits[3] = 0;
          set_scale(dst, scale);
          set_sign(dst, sign);
        }
      }
    }
  }

  return status;
}