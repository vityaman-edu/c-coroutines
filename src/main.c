#include <stdio.h>
#include <ucontext.h>

/// @see https://habr.com/ru/articles/519464/
static ucontext_t caller_context;
static ucontext_t coroutine_context;

void print_hello_and_suspend() {
  // выводим Hello
  printf("Hello");
  // точка передачи управления вызывающей стороне,
  // переключаемся на контекст caller_context
  // в контексте сопрограммы coroutine_context сохраняется текущая точка
  // выполнения, после возвращения контроля, выполнение продолжится с этой
  // точки.
  swapcontext(&coroutine_context, &caller_context);
}

void simple_coroutine() {
  // точка первой передачи управления в coroutine_context
  // чтобы продемонстрировать преимущества использование стека
  // выполним вложенный вызов функции print_hello_and_suspend.
  print_hello_and_suspend();
  // функция print_hello_and_suspend приостановила выполнение сопрограммы
  // после того как управление вернётся мы выведем Coroutine! и завершим работу,
  // управление будет передано контексту,
  // указатель на который хранится в coroutine_context.uc_link, т.е.
  // caller_context
  printf("Coroutine!\n");
}

int main() {
  // Стек сопрограммы.
  char stack[256];

  // Инициализация контекста сопрограммы coroutine_context
  // uc_link указывает на caller_context, точку возврата при завершении
  // сопрограммы. uc_stack хранит указатель и размер стека
  coroutine_context.uc_link = &caller_context;
  coroutine_context.uc_stack.ss_sp = stack;
  coroutine_context.uc_stack.ss_size = sizeof(stack);
  getcontext(&coroutine_context);

  // Заполнение coroutine_context
  // Контекст настраивается таким образом, что переключаясь на него
  // исполнение начинается с точки входа в функцию simple_coroutine
  makecontext(&coroutine_context, simple_coroutine, 0);

  // передаем управление сопрограмме, переключаемся на контекст
  // coroutine_context в контексте caller_context сохраняется текущая точка
  // выполнения, после возвращения контроля, выполнение продолжится с этой
  // точки.
  swapcontext(&caller_context, &coroutine_context);
  // сопрограмма приостановила свое выполнение и вернула управление
  // выводим пробел
  printf(" ");
  // передаём управление обратно сопрограмме.
  swapcontext(&caller_context, &coroutine_context);

  return 0;
}
