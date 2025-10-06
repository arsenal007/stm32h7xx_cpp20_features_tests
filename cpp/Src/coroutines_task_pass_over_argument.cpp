#include <coroutine>
#include <optional>
#include <cstdio>
#include <coroutines_task.h>
#include <simple.hpp> // execute::simple
#include <handler.hpp>
#include <runtime_initial_suspend.hpp>
#include "tim.h"

namespace task_example
{
  // --- awaitable ---
  struct Event
  {
    std::optional<int> value{};
    std::coroutine_handle<> waiter{};

  
    bool await_ready() const noexcept { return value.has_value(); }
    void await_suspend(std::coroutine_handle<> h) noexcept { waiter = h; }

    int await_resume() noexcept
    {
      int v = *value;
      value.reset();
      return v;
    }

    // called from "task": put a value and wake up the coroutine
    void send(int v)
    {
      value = v;
      if (waiter)
      {
        auto h = waiter;
        waiter = {};
        h.resume();
      }
    }
  };

  struct Task
  {
    struct promise_type
    {
      Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
      std::suspend_never initial_suspend(void) noexcept { return {}; }
      std::suspend_always final_suspend(void) noexcept { return {}; }
      void return_void(void) {}
      void unhandled_exception(void) { std::terminate(); }
    };

    using handle_t = std::coroutine_handle<promise_type>;
    std::coroutine_handle<promise_type> h{};

    explicit Task(handle_t hh) : h(hh) {}
    Task(Task &&o) noexcept : h(o.h) { o.h = {}; }
    Task(const Task &) = delete;
    ~Task()
    {
      if (h)
        h.destroy();
    }
  };

  // Coroutine: waits for data via co_await Event and prints it
  Task counter(Event &event)
  {
    for (;;)
    {
      int x = co_await event;
      printf("input value: %d\n", x);
    }
  }

  void execute(void)
  {
    Event ev{};
    Task task = counter(ev);

    for (int i = 0; i < 17; ++i)
    {
      printf("In task \n");
      ev.send(i); // send the value and wake up the coroutine
    }
  }
}

void coroutines_task(void)
{
  simple::execute();
  runtime_initial_suspend::execute();
  task_example::execute();
  HAL_TIM_Base_Start_IT(&htim7); // start timer with interrupt
}
