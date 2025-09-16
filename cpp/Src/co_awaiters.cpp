#include <coroutine>
#include <optional>
#include <cstdio>
#include <coroutines_task.h>

namespace simple
{
  class Handler
  {
  public:
    class promise_type;
    class Await;

    explicit Handler(std::coroutine_handle<promise_type> h) : handle_(h) {}
    ~Handler()
    {
      if (handle_)
        handle_.destroy();
    }

  private:
    std::coroutine_handle<promise_type> handle_;
  };

  class Handler::Await
  {
  public:
    Await(bool ready) : ready_(ready) {}
    bool await_ready() const noexcept { return ready_; }
    void await_suspend(std::coroutine_handle<promise_type>) const noexcept {}
    void await_resume() const noexcept {}

  private:
    bool ready_;
  };
  class Handler::promise_type
  {
  public:
    promise_type(bool ready) : ready_{ready} {}

    Handler get_return_object() { return Handler{std::coroutine_handle<promise_type>::from_promise(*this)}; }
    Await initial_suspend() { return Await{ready_}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}

  private:
    bool ready_;
  };

  Handler hello_coro(bool)
  {
    printf("Hello from coroutine! Handler type\n");
    co_return;
  }

  void execute(void)
  {
    Handler handler_await = hello_coro(true);
    Handler handler_run = hello_coro(false);
  }
}

namespace task_example
{
  struct Event
  {
    std::optional<int> value{};
    std::coroutine_handle<> waiter{};

    // --- awaitable interface ---
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
      Task get_return_object()
      {
        return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
      }
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
  task_example::execute();
}
