#include "coroutines/tasks/delayed.hpp"
#include <memory>
#include <coroutine>
#include <cstdio>
#include <handler.hpp>
#include <promise.hpp>
#include <delayed_execution.hpp>
#include <FreeRTOS.h>

namespace
{
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
        delayed_execution([h](void) -> void
                          { if (h) h.resume(); });
    }

    Handler<promise> delayed_coroutine(void)
    {
        printf("Starting delayed coroutine:\n");
        co_await Awaitable{};
        printf("Delayed execution after 5 seconds:\n");
        if (xPortIsInsideInterrupt())
            printf("interrupt contex\n");
        else
            printf("Delayed execution after 5 seconds from task context\n");
    }

    std::unique_ptr<Handler<promise>> handler_ptr{};
}

namespace coroutines
{
    namespace tasks
    {
        void delayed(void)
        {
            handler_ptr = std::make_unique<Handler<promise>>(delayed_coroutine()); // call coroutine
        }
    }
}
