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

#define coroutine_declare(yield_type, name, members)                           \
  struct coroutine_##name {                                                    \
    int label;                                                                 \
    members;                                                                   \
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

coroutine_declare(int, hello_world, int i);

coroutine_define(int, hello_world) {
  coroutine_start();
  printf("Hello\n");
  ++this->i;
  coroutine_yield(this->i);
  printf(",\n");
  ++this->i;
  coroutine_yield(this->i);
  printf("World\n");
  ++this->i;
  coroutine_yield(this->i);
  printf("!\n");
  ++this->i;
  coroutine_finish(this->i);
}

int main() {
  coroutine_create(hello, hello_world);
  printf("yeilds %d\n", coroutine_next(hello));
  printf("yeilds %d\n", coroutine_next(hello));
  printf("yeilds %d\n", coroutine_next(hello));
  printf("yeilds %d\n", coroutine_next(hello));
  printf("yeilds %d\n", coroutine_next(hello));
  printf("label = %d\n", hello.label);
}
