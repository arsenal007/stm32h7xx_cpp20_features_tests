

#include <cstdio>
#include <coroutine>
#include <handler.hpp>
#include <runtime_initial_suspend.hpp>

#include "cmsis_os.h"

namespace runtime_initial_suspend
{
    class RuntimeAwait
    {
    public:
        RuntimeAwait(bool ready) : ready_(ready) {}
        bool await_ready() const noexcept { return ready_; }
        template <typename T>
        void await_suspend(std::coroutine_handle<T>) const noexcept {}
        void await_resume() const noexcept {}

    private:
        bool ready_;
    };
    class DynamicPromise
    {
    public:
        DynamicPromise(bool ready) : ready_{ready} {}

        Handler<DynamicPromise> get_return_object() { return Handler<DynamicPromise>{std::coroutine_handle<DynamicPromise>::from_promise(*this)}; }
        RuntimeAwait initial_suspend() { return RuntimeAwait{ready_}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}

    private:
        bool ready_;
    };

    Handler<DynamicPromise> runtime_initial_suspend(bool ready)
    {
        if (ready)
            printf("Runtime initial suspend coroutine: await_ready = true \n");
        else
            printf("Runtime initial suspend coroutine: await_ready = false \n");
        co_return;
    }

    void execute(void)
    {
        Handler<DynamicPromise> handler_await = runtime_initial_suspend(true);
        Handler<DynamicPromise> handler_run = runtime_initial_suspend(false);
    }
}