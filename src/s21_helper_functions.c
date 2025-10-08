#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"

int s21_is_zero(s21_decimal d) {
  return d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0;
}

int get_sign(s21_decimal d) { return ((unsigned int)d.bits[3] >> 31) & 1; }

int get_scale(s21_decimal d) { return ((unsigned int)d.bits[3] >> 16) & 0xFF; }

void set_sign(s21_decimal *d, int sign) {
  unsigned int value = (unsigned int)d->bits[3];
  if (sign) {
    value |= (1U << 31);
  } else {
    value &= ~(1U << 31);
  }
  d->bits[3] = (int)value;
}

void set_scale(s21_decimal *d, int scale) {
  unsigned int value = (unsigned int)d->bits[3];
  int sign = (value >> 31) & 1;
  value = (sign << 31) | ((unsigned int)scale << 16);
  d->bits[3] = (int)value;
}

int s21_div_by_10(s21_decimal *dec, int *scale) {
  uint64_t remainder = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t temp =
        ((uint64_t)remainder << 32) | (uint64_t)(unsigned int)dec->bits[i];
    dec->bits[i] = (int)(temp / 10);
    remainder = temp % 10;
  }
  (*scale)--;
  return (int)remainder;
}

int s21_mul_10(s21_decimal *d) {
  unsigned int l = (unsigned int)d->bits[0];
  unsigned int m = (unsigned int)d->bits[1];
  unsigned int h = (unsigned int)d->bits[2];
  unsigned long long tmp;
  unsigned int new_l, new_m, new_h;
  unsigned long long carry = 0;
  int result = 0;

  tmp = (unsigned long long)l * 10;
  new_l = (unsigned int)(tmp & 0xFFFFFFFFU);
  carry = tmp >> 32;

  tmp = (unsigned long long)m * 10 + carry;
  new_m = (unsigned int)(tmp & 0xFFFFFFFFU);
  carry = tmp >> 32;

  tmp = (unsigned long long)h * 10 + carry;
  new_h = (unsigned int)(tmp & 0xFFFFFFFFU);
  carry = tmp >> 32;

  if (carry) {
    result = 1;
  } else {
    d->bits[0] = (int)new_l;
    d->bits[1] = (int)new_m;
    d->bits[2] = (int)new_h;
  }
  return result;
}

int s21_mul_10_exponent(s21_decimal *d, int exponent) {
  int old_scale = get_scale(*d);
  int sign = get_sign(*d);
  int overflow = 0;

  for (int i = 0; i < exponent && !overflow; i++) {
    overflow = s21_mul_10(d);
  }

  if (!overflow) {
    set_scale(d, old_scale + exponent);
  }
  set_sign(d, sign);
  return overflow;
}

int s21_compare_bits(s21_decimal a, s21_decimal b) {
  int result = 0;
  int high_compare = ((unsigned int)a.bits[2] > (unsigned int)b.bits[2]) -
                     ((unsigned int)a.bits[2] < (unsigned int)b.bits[2]);
  int mid_compare = ((unsigned int)a.bits[1] > (unsigned int)b.bits[1]) -
                    ((unsigned int)a.bits[1] < (unsigned int)b.bits[1]);
  int low_compare = ((unsigned int)a.bits[0] > (unsigned int)b.bits[0]) -
                    ((unsigned int)a.bits[0] < (unsigned int)b.bits[0]);

  if (high_compare) {
    result = high_compare;
  } else if (mid_compare) {
    result = mid_compare;
  } else {
    result = low_compare;
  }
  return result;
}

int s21_is_decimal(s21_decimal value) {
  int status = TRUE;
  s21_ctrl cw = {.word = value.bits[3]};
  if (cw.reserved_lo != 0 || cw.reserved_hi != 0) status = FALSE;
  if (status && cw.scale > 28) status = FALSE;
  return status;
}

void s21_copy_decimal(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
}

int s21_align_scale(s21_decimal *value_1, s21_decimal *value_2) {
  int status = OK;

  unsigned scale_1 = get_scale(*value_1);
  unsigned scale_2 = get_scale(*value_2);

  while (status == OK && scale_1 != scale_2) {
    if (scale_1 < scale_2) {
      s21_decimal tmp = *value_1;
      status = s21_mul_10(&tmp);
      if (status == OK) {
        *value_1 = tmp;
        ++scale_1;
        set_scale(value_1, scale_1);
      }
    } else {
      s21_decimal tmp = *value_2;
      status = s21_mul_10(&tmp);
      if (status == OK) {
        *value_2 = tmp;
        ++scale_2;
        set_scale(value_2, scale_2);
      }
    }
  }
  return status;
}

void s21_set_zero(s21_decimal *value) {
  value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
}

void mul_96x96_to_192(const s21_decimal *a, const s21_decimal *b,
                      unsigned int res[6]) {
  memset(res, 0, 6 * sizeof(unsigned int));

  for (int i = 0; i < 3; ++i) {
    unsigned long long carry = 0;
    for (int j = 0; j < 3; ++j) {
      unsigned long long tmp =
          (unsigned long long)a->bits[i] * (unsigned long long)b->bits[j] +
          res[i + j] + carry;
      res[i + j] = (unsigned int)tmp;
      carry = tmp >> 32;
    }
    res[i + 3] += (unsigned int)carry;
  }
}

unsigned div_192_by_10(unsigned int w[6]) {
  unsigned long long rem = 0;
  for (int i = 5; i >= 0; --i) {
    unsigned long long cur = (rem << 32) | w[i];
    w[i] = (unsigned int)(cur / 10u);
    rem = cur % 10u;
  }
  return (unsigned)rem;
}

int fits_96(unsigned int words[6]) {
  return (words[3] == 0 && words[4] == 0 && words[5] == 0);
}

int add1_to_192(unsigned int words[6]) {
  for (int i = 0; i < 6; ++i) {
    if (++words[i] != 0) return 0;
  }
  return 1;
}

unsigned int to_unsigned(int value) { return (unsigned int)value; }

int to_signed(unsigned int value) { return (int)value; }

int add_mantissa(s21_decimal *value_1, s21_decimal *value_2,
                 s21_decimal *result) {
  unsigned long carry = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long sum = (unsigned long)to_unsigned(value_1->bits[i]) +
                        (unsigned long)to_unsigned(value_2->bits[i]) + carry;
    result->bits[i] = to_signed((unsigned int)(sum & 0xFFFFFFFF));
    carry = sum >> 32;
  }

  return (carry != 0) ? NUMBER_IS_TOO_BIG : OK;
}

int compare_mantissa(const s21_decimal *value_1, const s21_decimal *value_2) {
  for (int i = 2; i >= 0; i--) {
    unsigned int uvalue_1 = to_unsigned(value_1->bits[i]);
    unsigned int uvalue_2 = to_unsigned(value_2->bits[i]);

    if (uvalue_1 > uvalue_2) return 1;
    if (uvalue_1 < uvalue_2) return -1;
  }
  return 0;
}

int subtract_mantissa(const s21_decimal *value_1, const s21_decimal *value_2,
                      s21_decimal *result) {
  unsigned long borrow = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long uvalue_1 = (unsigned long)to_unsigned(value_1->bits[i]);
    unsigned long uvalue_2 = (unsigned long)to_unsigned(value_2->bits[i]);

    if (uvalue_1 >= uvalue_2 + borrow) {
      result->bits[i] = to_signed((unsigned int)(uvalue_1 - uvalue_2 - borrow));
      borrow = 0;
    } else {
      result->bits[i] = to_signed(
          (unsigned int)(uvalue_1 + 0x100000000ULL - uvalue_2 - borrow));
      borrow = 1;
    }
  }

  return (borrow != 0) ? 1 : 0;
}

int increment_mantissa(s21_decimal *d) {
  unsigned long carry = 1;

  for (int i = 0; i < 3; i++) {
    unsigned long sum = to_unsigned(d->bits[i]) + carry;
    d->bits[i] = to_signed((unsigned int)(sum & 0xFFFFFFFF));
    carry = sum >> 32;
    if (carry == 0) break;
  }

  return (carry != 0) ? 1 : 0;
}

int is_mantissa_even(const s21_decimal *d) {
  return (to_unsigned(d->bits[0]) & 1) == 0;
}

int banker_round(s21_decimal *d) {
  int scale = get_scale(*d);

  if (scale == 0) {
    return 0;
  }

  int remainder = s21_div_by_10(d, &scale);
  set_scale(d, scale);

  if (remainder > 5) {
    return increment_mantissa(d);
  } else if (remainder < 5) {
    return 0;
  } else {
    if (!is_mantissa_even(d)) {
      return increment_mantissa(d);
    }
    return 0;
  }
}

int s21_add_abs(s21_decimal a, s21_decimal b, s21_decimal *result) {
  if (!result) return CALCULATION_ERROR;
  s21_set_zero(result);

  unsigned int carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long sum =
        (unsigned long long)a.bits[i] + (unsigned long long)b.bits[i] + carry;
    result->bits[i] = (unsigned int)(sum & 0xFFFFFFFF);
    carry = (unsigned int)(sum >> 32);
  }
  return carry ? CALCULATION_ERROR : OK;
}

int s21_sub_abs(s21_decimal a, s21_decimal b, s21_decimal *result) {
  if (!result) return CALCULATION_ERROR;
  s21_set_zero(result);

  int borrow = 0;
  for (int i = 0; i < 3; i++) {
    long long diff = (long long)a.bits[i] - b.bits[i] - borrow;
    if (diff < 0) {
      borrow = 1;
      diff += 4294967296LL;
    } else {
      borrow = 0;
    }
    result->bits[i] = (unsigned int)diff;
  }
  return borrow ? CALCULATION_ERROR : OK;
}