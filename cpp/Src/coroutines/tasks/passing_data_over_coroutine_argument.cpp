#include "coroutines/tasks/passing_data_over_coroutine_argument.hpp"
#include <coroutine>
#include <optional>
#include <cstdio>
#include <coroutines_task.h>
#include <simple.hpp> // execute::simple
#include <handler.hpp>
#include <promise.hpp>
#include <runtime_initial_suspend.hpp>
#include <memory>

namespace
{
  // --- awaitable ---
  struct EventAwaitable
  {
    std::optional<int32_t> value_{};
    handle_t waiter_{};

    bool await_ready() const noexcept { return value_.has_value(); }
    void await_suspend(handle_t h) noexcept { waiter_ = h; }

    int await_resume() noexcept
    {
      int32_t v = *value_;
      value_.reset();
      return v;
    }

    // called from "task": put a value and wake up the coroutine
    void send(int32_t v)
    {
      value_ = v;
      if (waiter_)
      {
        auto h = waiter_;
        waiter_ = {};
        h.resume();
      }
    }
  };

  // Coroutine: waits for data via co_await Event and prints it
  Handler<promise> counter(EventAwaitable &event)
  {
    for (;;)
    {
      int x = co_await event;
      printf("input value: %d\n", x);
    }
  }

  std::unique_ptr<Handler<promise>> handler_ptr{};
}

namespace coroutines
{
  namespace tasks
  {
    void passing_data_over_coroutine_argument(void)
    {
      EventAwaitable ev{};
      handler_ptr = std::make_unique<Handler<promise>>(counter(ev));

      for (int i = 0; i < 17; ++i)
      {
        printf("In task \n");
        ev.send(i); // send the value and wake up the coroutine
      }
    }
  }
}
