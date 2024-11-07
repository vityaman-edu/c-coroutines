#include "foreach.h"

#define coroutine_load(id) typeof(this->id) id = this->id;

#define coroutine_store(id) this->id = id;

#define coroutine_start(...)                                                   \
  FOR_EACH(coroutine_load, __VA_ARGS__)                                        \
  switch (this->label) {                                                       \
  case 0:

#define coroutine_yield(val)                                                   \
  this->label = __LINE__;                                                      \
  this->yield = val;                                                           \
  goto yield;                                                                  \
  case __LINE__:

#define coroutine_finish(val, ...)                                             \
  default:                                                                     \
    this->label = -1;                                                          \
    this->yield = val;                                                         \
    goto yield;                                                                \
    }                                                                          \
  yield:                                                                       \
    FOR_EACH(coroutine_store, __VA_ARGS__)                                     \
    return this->yield;

#define coroutine_declare(yield_type, name, ...)                               \
  struct coroutine_##name {                                                    \
    int label;                                                                 \
    yield_type yield;                                                          \
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
