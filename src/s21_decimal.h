#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
  int bits[4];
} s21_decimal;

enum {
  OK = 0,
  NUMBER_IS_TOO_BIG = 1,
  NUMBER_IS_TOO_SMALL = 2,
  DIVISION_BY_ZERO = 3,
  PLUS = 0,
  MINUS = 1,
  FALSE = 0,
  TRUE = 1,
  CONVERTATION_ERROR = 1,
  CALCULATION_ERROR = 1
};

typedef union {
  unsigned word;
  struct {
    unsigned reserved_lo : 16;
    unsigned scale : 8;
    unsigned reserved_hi : 7;
    unsigned sign : 1;
  };
} s21_ctrl;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif