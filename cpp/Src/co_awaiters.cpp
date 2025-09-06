#include <coroutine>
#include <optional>
#include <cstdio>
#include <coroutines_task.h>

struct Event
{
  std::optional<int> value{};
  std::coroutine_handle<> waiter{};

  // --- awaitable інтерфейс ---
  bool await_ready() const noexcept { return value.has_value(); }
  void await_suspend(std::coroutine_handle<> h) noexcept { waiter = h; }
  int await_resume() noexcept
  {
    int v = *value;
    value.reset();
    return v;
  }

  // викликаємо з "task": покласти значення і розбудити корутину
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
    Task get_return_object()
    {
      return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  using handle_t = std::coroutine_handle<promise_type>;
  handle_t h{};

  explicit Task(handle_t hh) : h(hh) {}
  Task(Task &&o) noexcept : h(o.h) { o.h = {}; }
  Task(const Task &) = delete;
  ~Task()
  {
    if (h)
      h.destroy();
  }


};

// Корутина: чекає дані через co_await Event і друкує їх
Task counter(Event &ev)
{
  for (;;)
  {
    int x = co_await ev;
    printf("counter: %d\n", x);
  }
}

void coroutines_task(void)
{
  Event ev;
  auto c = counter(ev);

  for (int i = 0; i < 3; ++i)
  {
    printf("In task \n");
    ev.send(i); // передаємо значення і будимо корутину
  }
}
