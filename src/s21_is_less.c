#include "s21_decimal.h"
#include "s21_helper_functions.h"

int s21_is_less(s21_decimal first, s21_decimal second) {
  int status = FALSE;
  if (!s21_is_decimal(first) || !s21_is_decimal(second)) {
    status = FALSE;
  } else {
    if ((first.bits[0] == 0 && first.bits[1] == 0 && first.bits[2] == 0) &&
        (second.bits[0] == 0 && second.bits[1] == 0 && second.bits[2] == 0)) {
      status = FALSE;
    } else {
      unsigned sign_first = get_sign(first);
      unsigned sign_second = get_sign(second);
      if (sign_first != sign_second) {
        status = (sign_first > sign_second);
      } else {
        if (s21_align_scale(&first, &second) == OK) {
          int cmp = 0;
          for (int i = 2; i >= 0 && cmp == 0; --i) {
            if (first.bits[i] != second.bits[i])
              cmp = (first.bits[i] > second.bits[i]) ? 1 : -1;
          }
          if (cmp != 0) {
            status = (sign_first == 0) ? (cmp < 0) : (cmp > 0);
          }
        }
      }
    }
  }
  return status;
}