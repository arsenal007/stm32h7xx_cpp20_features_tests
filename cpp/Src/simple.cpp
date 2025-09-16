#include <coroutine>
#include <optional>
#include <cstdio>
#include <coroutines_task.h>
#include <handler.hpp>
#include <simple.hpp>

namespace simple
{
  // promise type suspend_always
  class suspend_always
  {
  public:
    Handler<suspend_always> get_return_object() { return Handler<suspend_always>{std::coroutine_handle<suspend_always>::from_promise(*this)}; }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };

  // promise type suspend_never
  class suspend_never
  {
  public:
    Handler<suspend_never> get_return_object() { return Handler<suspend_never>{std::coroutine_handle<suspend_never>::from_promise(*this)}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };



  Handler<suspend_always> initial_suspend_always(void)
  {
    printf("This message should not be shown because we suspend it at the initial stage\n");
    co_return;
  }

  Handler<suspend_never> initial_suspend_never(void)
  {
    printf("Hello World from coroutine!\n");
    co_return;
  }


  void execute(void)
  {
    Handler<suspend_always> handler_initially_suspended = initial_suspend_always();
    Handler<suspend_never> handler_initially_not_suspended = initial_suspend_never();
  }
}
