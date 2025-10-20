#include <coroutine>
#include <optional>
#include <cstdio>
#include <memory>
#include <coroutines/task_passing_data_over_promise.hpp>

namespace
{
  class promise;

  using handle_t = std::coroutine_handle<promise>;

  // --- awaitable interface ---
  class Awaiatable
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

  class promise
  {
  public:
    TaskHandler get_return_object() { return TaskHandler{handle_t::from_promise(*this)}; }
    std::suspend_never initial_suspend(void) noexcept { return {}; }
    std::suspend_always final_suspend(void) noexcept { return {}; }
    void return_void(void) {}
    void unhandled_exception(void) { std::terminate(); }

    std::optional<int> value{};
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
    handler_.promise().value = v;
    handler_.resume();
  }

  bool Awaiatable::await_ready() const noexcept
  {
    if (!Awaiatable::handler_)
      return false;
    return Awaiatable::handler_.promise().value.has_value();
  }

  void Awaiatable::await_suspend(handle_t h) noexcept { Awaiatable::handler_ = h; }

  int Awaiatable::await_resume() noexcept
  {
    int v = *Awaiatable::handler_.promise().value;
    Awaiatable::handler_.promise().value.reset();
    Awaiatable::handler_ = {};
    return v;
  }

  // Coroutine: waits for data via co_await Event and prints it
  TaskHandler counter(void)
  {
    for (;;)
    {
      int x = co_await Awaiatable{};
      printf("passing data over promise input value inside coroutine: %d\n", x);
    }
  }

  std::unique_ptr<TaskHandler> task_handler;
}

void coroutine_task_example_passing_data_over_promise(void)
{

  task_handler = std::make_unique<TaskHandler>(counter());
  for (int i = 0; i < 17; ++i)
  {
    printf("execution loop on callable side\n");
    task_handler->send(i); // send the value and wake up the coroutine
  }
}
