#include <assert.h>
#include <stdio.h>

#define coroutine_start()                                                      \
  switch (this->state.label) {                                                 \
  case 0: {

#define coroutine_yield()                                                      \
  this->state.label = __LINE__;                                                \
  }                                                                            \
  break;                                                                       \
  case __LINE__: {

#define coroutine_finish()                                                     \
  this->state.label = -1;                                                      \
  }                                                                            \
  break;                                                                       \
  }

#define coroutine_define(name) void name(struct coroutine *this)

#define coroutine_create(name, type)                                           \
  struct coroutine name = coroutine_new(&type)

#define coroutine_next(instance) instance.procedure(&instance)

struct coroutine {
  struct {
    int label;
  } state;
  void (*const procedure)(struct coroutine *);
};

struct coroutine coroutine_new(void (*procedure)(struct coroutine *)) {
  return (struct coroutine){
      .state.label = 0,
      .procedure = procedure,
  };
}

coroutine_define(hello_world) {
  coroutine_start();
  printf("Hello");
  coroutine_yield();
  printf(",");
  coroutine_yield();
  printf(" ");
  coroutine_yield();
  printf("World");
  coroutine_yield();
  printf("!");
  coroutine_yield();
  printf("\n");
  coroutine_finish();
}

int main() {
  coroutine_create(hello, hello_world);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  coroutine_next(hello);
  printf("label = %d\n", hello.state.label);
}
