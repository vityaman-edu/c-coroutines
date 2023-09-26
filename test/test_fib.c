#include "coroutine.h"
#include <assert.h>
#include <stdio.h>

coroutine_declare(int, fibb, const int n; int prev; int next; int i);

coroutine_define(int, fibb) {
  coroutine_start(i, prev, next, n);
  i = 2;
  prev = 1;
  next = 1;
  coroutine_yield(prev);
  coroutine_yield(next);
  while (i < n) {
    int curr = prev + next;
    prev = next;
    next = curr;
    i += 1;
    coroutine_yield(curr);
  }
  coroutine_finish(0, i, prev, next);
}

int main() {
  int expected[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144};
  
  coroutine_create(fibs, fibb, .n = 12);
  for (int i = 0; i < (int)(sizeof(expected) / sizeof(int)); ++i) {
    const int next = coroutine_next(fibs);
    if (next == 0) {
      break;
    }
    if (next != expected[i]) {
        printf("%d != %d\n", next, expected[i]);
    }
    assert(next == expected[i]);    
  }

  for (int i = 0; i < 5; ++i) {
    assert(coroutine_next(fibs) == 0);
  }
}
