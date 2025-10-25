#include <coroutine>
#include <optional>
#include <cstdio>
#include <memory>
#include <coroutines/tasks/passing_data_over_promise.hpp>
#include <coroutines/value_promise.hpp>

namespace
{

  class TaskHandler;
  using promise = value_promise<TaskHandler>;
  using handle_t = std::coroutine_handle<promise>;

  // --- awaitable interface ---
  class Awaitable
  {
  public:
    bool await_ready() const noexcept;
    void await_suspend(handle_t h) noexcept;
    int await_resume() noexcept;

  private:
    handle_t handler_{};
  };

  class TaskHandler
  {
  public:
    using promise_type = promise;

    // used in get_return_object
    explicit TaskHandler(handle_t);

    // used in std::make_unique
    TaskHandler(TaskHandler &&o);

    TaskHandler(const TaskHandler &) = delete;
    TaskHandler &operator=(const TaskHandler &) = delete;
    TaskHandler &operator=(TaskHandler &&o) = delete;

    // destroy handler
    ~TaskHandler(void);

    // called from "task": put a value and wake up the coroutine
    void send(int v);

  private:
    handle_t handler_{};
  };

  TaskHandler::TaskHandler(handle_t h) : handler_(h) {}

  TaskHandler::TaskHandler(TaskHandler &&o) : handler_(o.handler_)
  {
    o.handler_ = {};
  }

  TaskHandler::~TaskHandler()
  {
    if (handler_)
      handler_.destroy();
  }

  // called from "task": put a value and wake up the coroutine
  void TaskHandler::send(int v)
  {
    handler_.promise().value_ = v;
    handler_.resume();
  }

  bool Awaitable::await_ready() const noexcept
  {
    if (!handler_)
      return false;
    return handler_.promise().value_.has_value();
  }

  void Awaitable::await_suspend(handle_t h) noexcept { handler_ = h; }

  int Awaitable::await_resume() noexcept
  {
    int v = *handler_.promise().value_;
    handler_.promise().value_.reset();
    handler_ = {};
    return v;
  }

  // Coroutine: waits for data via co_await Event and prints it
  TaskHandler counter(void)
  {
    for (;;)
    {
      int x = co_await Awaitable{};
      printf("passing data over promise input value inside coroutine: %d\n", x);
    }
  }

  std::unique_ptr<TaskHandler> task_handler{};
}

namespace coroutines
{
  namespace tasks
  {
    void passing_data_over_promise(void)
    {

      task_handler = std::make_unique<TaskHandler>(counter());
      for (int i = 0; i < 17; ++i)
      {
        printf("execution loop on callable side\n");
        task_handler->send(i); // send the value and wake up the coroutine
      }
    }
  }
}