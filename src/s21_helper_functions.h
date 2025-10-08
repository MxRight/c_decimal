#ifndef S21_HELPER_FUNCTIONS_H
#define S21_HELPER_FUNCTIONS_H

#include "s21_decimal.h"

int s21_is_zero(s21_decimal d);
int get_sign(s21_decimal d);
int get_scale(s21_decimal d);
void set_sign(s21_decimal *d, int sign);
void set_scale(s21_decimal *d, int scale);
int s21_div_by_10(s21_decimal *dec, int *scale);
int s21_mul_10(s21_decimal *d);
int s21_mul_10_exponent(s21_decimal *d, int exponent);
int s21_compare_bits(s21_decimal a, s21_decimal b);
int s21_is_decimal(s21_decimal value);
void s21_copy_decimal(s21_decimal value, s21_decimal *result);
int s21_align_scale(s21_decimal *value_1, s21_decimal *value_2);
void s21_set_zero(s21_decimal *value);
void mul_96x96_to_192(const s21_decimal *a, const s21_decimal *b,
                      unsigned int res[6]);
unsigned div_192_by_10(unsigned int w[6]);
int fits_96(unsigned int words[6]);
int add1_to_192(unsigned int words[6]);

int banker_round(s21_decimal *d);
int increment_mantissa(s21_decimal *d);
int subtract_mantissa(const s21_decimal *value_1, const s21_decimal *value_2,
                      s21_decimal *result);
int compare_mantissa(const s21_decimal *value_1, const s21_decimal *value_2);
int add_mantissa(s21_decimal *value_1, s21_decimal *value_2,
                 s21_decimal *result);
int is_mantissa_even(const s21_decimal *d);

int to_signed(unsigned int value);
unsigned int to_unsigned(int value);

int s21_add_abs(s21_decimal a, s21_decimal b, s21_decimal *result);
int s21_sub_abs(s21_decimal a, s21_decimal b, s21_decimal *result);

#endif