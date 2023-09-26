#include <assert.h>
#include <stdio.h>

#define BLANK \

#define coroutine_start()                                                      \
  switch (this->state.label) {                                                 \
  case 0: {

#define coroutine_yield()                                                      \
  this->state.label = __LINE__;                                            \
  }                                                                            \
  break;                                                                       \
  case __LINE__: {

#define coroutine_finish()                                                     \
  this->state.label = -1;                                                      \
  }                                                                            \
  break;                                                                       \
  }

struct coroutine {
  struct {
    int label;
  } state;
};

void hello_world(struct coroutine *this) {
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
  struct coroutine hello;
  hello.state.label = 0;
  hello_world(&hello);
  hello_world(&hello);
  hello_world(&hello);
  hello_world(&hello);
  hello_world(&hello);
  hello_world(&hello);
  printf("label = %d\n", hello.state.label);
}
