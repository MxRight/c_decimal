#include <stdio.h>

#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = OK;
  if (result == NULL || s21_is_decimal(value) == FALSE) {
    status = CALCULATION_ERROR;
  } else {
    s21_copy_decimal(value, result);
    result->bits[3] ^= (1u << 31);
  }

  return status;
}