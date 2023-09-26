#include "coroutine.h"
#include <assert.h>

static int step = 0;

coroutine_declare(int, mutator);

coroutine_define(int, mutator) {
  coroutine_start();
  step = 1;
  coroutine_yield(11);
  step = 2;
  coroutine_yield(22);
  step = 3;
  coroutine_yield(33);
  step = 4;
  coroutine_finish(44);
}

int main() {
  coroutine_create(steps, mutator);
  assert(step == 0);
  assert(coroutine_next(steps) == 11);
  assert(step == 1);
  assert(coroutine_next(steps) == 22);
  assert(step == 2);
  assert(coroutine_next(steps) == 33);
  assert(step == 3);
  assert(coroutine_next(steps) == 44);
  assert(step == 4);
  assert(coroutine_next(steps) == 44);
  assert(step == 4);
  assert(coroutine_next(steps) == 44);
  assert(step == 4);
  return 0;
}
