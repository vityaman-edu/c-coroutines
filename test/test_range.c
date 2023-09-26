#include "coroutine.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

coroutine_declare(int, range, const int lower; const int upper; int i);

coroutine_define(int, range) {
  coroutine_start(i, lower, upper);
  for (i = lower; i < upper; ++i) {
    coroutine_yield(i);
  }
  coroutine_finish(upper, i);
}

int main() {
  const int lower = 5;
  const int upper = 22;

  const int count = (upper - lower);
  int* const expected = malloc(sizeof(int) * count);
  for (int i = 0; i < count; ++i) {
    expected[i] = lower + i;
  }

  coroutine_create(numbers, range, .lower = lower, .upper = upper);
  for (int i = 0;; ++i) {
    const int actual = coroutine_next(numbers);
    if (actual == numbers.upper) {
      break;
    }
    assert(actual == expected[i]);
  }

  for (int i = 0; i < 5; ++i) {
    assert(coroutine_next(numbers) == numbers.upper);
  }

  free(expected);
  return 0;
}