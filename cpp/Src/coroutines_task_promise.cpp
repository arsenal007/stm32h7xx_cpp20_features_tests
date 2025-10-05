#include <coroutine>
#include <optional>
#include <cstdio>

namespace task_example_with_passing_data_over_promise
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

  class Task
  {
  public:
    using promise_type = promise;

    explicit Task(handle_t);
    Task(Task &&o) = delete;
    Task(const Task &) = delete;
    ~Task();

    // called from "task": put a value and wake up the coroutine
    void send(int v);

  private:
    handle_t handler{};
  };

  class promise
  {
  public:
    Task get_return_object() { return Task{handle_t::from_promise(*this)}; }
    std::suspend_never initial_suspend(void) noexcept { return {}; }
    std::suspend_always final_suspend(void) noexcept { return {}; }
    void return_void(void) {}
    void unhandled_exception(void) { std::terminate(); }

    std::optional<int> value{};
  };

  Task::Task(handle_t hh) : handler(hh) {}

  Task::~Task()
  {
    if (handler)
      handler.destroy();
  }

  // called from "task": put a value and wake up the coroutine
  void Task::send(int v)
  {
    handler.promise().value = v;
    handler.resume();
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
  Task counter(void)
  {
    for (;;)
    {
      int x = co_await Awaiatable{};
      printf("passing data over promise input value inside coroutine: %d\n", x);
    }
  }

  void execute(void)
  {

    Task task = counter();
    for (int i = 0; i < 17; ++i)
    {
      printf("execution loop on callable side\n");
      task.send(i); // send the value and wake up the coroutine
    }
  }
}
