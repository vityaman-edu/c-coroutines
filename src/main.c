#include <assert.h>
#include <stdio.h>

#define coroutine_start()                                                      \
  switch (this->label) {                                                       \
  case 0: {

#define coroutine_suspend()                                                    \
  this->label = __LINE__;                                                      \
  }                                                                            \
  break;                                                                       \
  case __LINE__: {

#define coroutine_yield(val)                                                   \
  this->label = __LINE__;                                                      \
  return val;                                                                  \
  }                                                                            \
  break;                                                                       \
  case __LINE__: {

#define coroutine_finish(val)                                                  \
  this->label = -1;                                                            \
  return val;                                                                  \
  }                                                                            \
  break;                                                                       \
  default: {                                                                   \
    this->label = -1;                                                          \
    return val;                                                                \
  } break;                                                                     \
    }

#define coroutine_declare(yield_type, name)                                    \
  struct coroutine_##name {                                                    \
    int label;                                                                 \
    yield_type (*const procedure)(struct coroutine_##name *);                  \
  };                                                                           \
                                                                               \
  yield_type name(struct coroutine_##name *this)

#define coroutine_define(yield_type, name)                                     \
  yield_type name(struct coroutine_##name *this)

#define coroutine_create(instance, name)                                       \
  struct coroutine_##name instance = (struct coroutine_##name) {               \
    .label = 0, .procedure = &name,                                            \
  }

#define coroutine_next(instance) instance.procedure(&instance)

coroutine_declare(void, hello_world);

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

int main() {
  coroutine_create(hello, hello_world);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  printf("label = %d\n", hello.label);
}
