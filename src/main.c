#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 1000

typedef struct {
  ucontext_t internal;
} coroutine_context;

typedef struct {
  coroutine_context caller_context;
  coroutine_context this_context;
} coroutine;

coroutine *coroutine_new(void (*procedure)(coroutine *)) {
  coroutine *coro = malloc(sizeof(coroutine));
  coro->this_context.internal.uc_link = &coro->caller_context.internal;
  coro->this_context.internal.uc_stack.ss_sp = malloc(STACK_SIZE);
  coro->this_context.internal.uc_stack.ss_size = STACK_SIZE;
  getcontext(&coro->this_context.internal);
  makecontext(&coro->this_context.internal, (void (*)(void))procedure, 1, coro);
  return coro;
}

void coroutine_yield(coroutine *coroutine) {
  swapcontext(&coroutine->this_context.internal,
              &coroutine->caller_context.internal);
}

void coroutine_next(coroutine *coroutine) {
  swapcontext(&coroutine->caller_context.internal,
              &coroutine->this_context.internal);
}

void coroutine_free(coroutine *coroutine) {
  // TODO(vityaman): free(): invalid pointer
  // free(coroutine->caller_context.internal.uc_stack.ss_sp);

  // TODO(vityaman): free(): invalid next size (normal)
  // free(coroutine);
}

void print_hello_and_suspend(coroutine *this) {
  printf("Hello");
  coroutine_yield(this);
  printf("hello");
  coroutine_yield(this);
}

void simple_coroutine(coroutine *this) {
  print_hello_and_suspend(this);
  printf("Coroutine!\n");
}

int main() {
  coroutine *coroutine = coroutine_new(simple_coroutine);
  coroutine_next(coroutine);
  printf("-");
  coroutine_next(coroutine);
  printf(" ");
  coroutine_next(coroutine);
  coroutine_free(coroutine);
  printf("We are still alive here\n");
  return 0;
}
