#include "parse_uart_command.h"
#include <string>
#include <cstdio>
#include "FreeRTOS.h"
#include <delayed_execution.hpp>
#include <coroutine>
#include <optional>
#include <handler.hpp>
#include <memory>
#include <simple.hpp>

#include <coroutines/task_passing_data_over_promise.hpp>

namespace
{

    class promise_t;

    using handle_t = std::coroutine_handle<promise_t>;
    class promise_t
    {
    public:
        Handler<promise_t> get_return_object(void) noexcept { return Handler<promise_t>(handle_t::from_promise(*this)); };
        std::suspend_never initial_suspend(void) noexcept { return {}; }
        std::suspend_always final_suspend(void) noexcept { return {}; }
        void return_void(void) {}
        void unhandled_exception(void) { std::terminate(); }
    };

    // --- awaitable interface ---
    class Awaitable
    {
    public:
        bool await_ready() const noexcept { return false; }
        void await_suspend(handle_t h) noexcept;
        void await_resume() noexcept {}
    };

    void Awaitable::await_suspend(handle_t h) noexcept
    {
        printf("Awaitable::await_suspend\n");
        delayed_execution([h]()
                          { if (h) h.resume(); });
    }

    std::unique_ptr<Handler<promise_t>> handler_ptr;

    Handler<promise_t> delayed(void)
    {
        printf("Starting delayed coroutine:\n");
        co_await Awaitable{};
        printf("Delayed execution after 5 seconds:\n");
        if (xPortIsInsideInterrupt())
            printf("interrupt contex\n");
        else
            printf("Delayed execution after 5 seconds from task context\n");
    }

    inline void handleCommand(const std::string cmd)
    {
        if (cmd == "CORO_SIMPLE")
        {
            simple::execute();
        }
        else if (cmd == "DELAYED")
        {
            delayed_execution([]()
                              { printf("Delayed execution after 5 seconds:\n"); 
                      if (xPortIsInsideInterrupt()) 
                        printf("interrupt contex\n"); 
                      else 
                        printf("Delayed execution after 5 seconds from task context\n"); });
        }
        else if (cmd == "CORO_DELAYED")
        {
            handler_ptr = std::make_unique<Handler<promise_t>>(delayed());
        }
        else if (cmd == "CORO_PASSING_DATA_OVER_PROMISE")
        {
            coroutine_task_example_passing_data_over_promise();
        }
        else
        {
            printf("Unknown command: %s\n", cmd.c_str());
        }
    }
}

void parse_uart_command(const char *input)
{
    handleCommand(input);
}