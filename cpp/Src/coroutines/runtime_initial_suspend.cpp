

#include <cstdio>
#include <coroutine>
#include <memory>
#include <handler.hpp>
#include <coroutines/runtime_initial_suspend.hpp>

#include "cmsis_os.h"

namespace
{
    class RuntimeAwait
    {
    public:
        RuntimeAwait(bool ready) : ready_(ready) {}
        bool await_ready() const noexcept { return ready_; }
        template <typename T>
        void await_suspend(std::coroutine_handle<T>) const noexcept {}
        void await_resume(void) const noexcept {}

    private:
        bool ready_;
    };
    class DynamicPromise
    {
    public:
        DynamicPromise(bool ready) : ready_{ready} {}

        Handler<DynamicPromise> get_return_object() { return Handler<DynamicPromise>{std::coroutine_handle<DynamicPromise>::from_promise(*this)}; }
        RuntimeAwait initial_suspend(void) { return RuntimeAwait{ready_}; }
        std::suspend_always final_suspend(void) noexcept { return {}; }
        void unhandled_exception(void) {}
        void return_void(void) {}

    private:
        bool ready_;
    };

    Handler<DynamicPromise> runtime_initial_suspend_coroutine(bool ready)
    {
        if (ready)
            printf("Runtime initial suspend coroutine: await_ready = true \n");
        else
            printf("Runtime initial suspend coroutine: await_ready = false \n");
        co_return;
    }

    std::unique_ptr<Handler<DynamicPromise>> handler_await_ptr{};
    std::unique_ptr<Handler<DynamicPromise>> handler_run_ptr{};
}

namespace coroutines
{
    void runtime_initital_suspend(void)
    {
        handler_await_ptr = std::make_unique<Handler<DynamicPromise>>(runtime_initial_suspend_coroutine(true));
        handler_run_ptr = std::make_unique<Handler<DynamicPromise>>(runtime_initial_suspend_coroutine(false));
    }
}