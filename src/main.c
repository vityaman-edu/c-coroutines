#include <stdbool.h>
#include <stdio.h>

#define coroutine_start()                                                      \
  switch (this->label) {                                                       \
  case 0:

#define coroutine_yield(val)                                                   \
  this->label = __LINE__;                                                      \
  return val;                                                                  \
  case __LINE__:

#define coroutine_finish(val)                                                  \
  this->label = -1;                                                            \
  return val;                                                                  \
  default:                                                                     \
    this->label = -1;                                                          \
    return val;                                                                \
    }

#define coroutine_declare(yield_type, name, ...)                               \
  struct coroutine_##name {                                                    \
    int label;                                                                 \
    __VA_ARGS__;                                                               \
    yield_type (*const procedure)(struct coroutine_##name *);                  \
  };                                                                           \
                                                                               \
  yield_type name(struct coroutine_##name *this)

#define coroutine_define(yield_type, name)                                     \
  yield_type name(struct coroutine_##name *this)

#define coroutine_create(instance, name, ...)                                  \
  struct coroutine_##name instance = (struct coroutine_##name) {               \
    .label = 0, .procedure = &name, __VA_ARGS__                                \
  }

#define coroutine_next(instance) instance.procedure(&instance)

coroutine_declare(void, hello_world, );

coroutine_define(void, hello_world) {
  coroutine_start();
  printf("Hello\n");
  coroutine_yield();
  printf(",\n");
  coroutine_yield();
  printf("World\n");
  coroutine_yield();
  printf("!\n");
  coroutine_finish();
}

coroutine_declare(int, range, int lower; int upper; int i);

coroutine_define(int, range) {
  coroutine_start();
  for (this->i = this->lower; this->i < this->upper; ++this->i) {
    coroutine_yield(this->i);
  }
  coroutine_finish(this->upper);
}

coroutine_declare(int, fibb, int prev; int next; int i; int n);

coroutine_define(int, fibb) {
  coroutine_start();
  this->i = 2;
  this->prev = 1;
  this->next = 1;
  coroutine_yield(this->prev);
  coroutine_yield(this->next);
  while (this->i < this->n) {
    int curr = this->prev + this->next;
    this->prev = this->next;
    this->next = curr;
    this->i += 1;
    coroutine_yield(curr);
  }
  coroutine_finish(0);
}

int main() {
  coroutine_create(hello, hello_world);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);

  coroutine_create(numbers, range, .lower = 0, .upper = 21);
  while (true) {
    const int number = coroutine_next(numbers);
    if (number == numbers.upper) {
      break;
    }
    printf("numbers yields %d\n", number);
  }

  coroutine_create(fibs, fibb, .n = 12);
  while (true) {
    const int next = coroutine_next(fibs);
    if (next == 0) {
        break;
    }
    printf("fibs yields %d\n", next);
  }
}
