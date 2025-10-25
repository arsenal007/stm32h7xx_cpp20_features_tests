#include <coroutines/generators/Awaitable.hpp>

#include <coroutine>
#include <optional>
#include <memory>
#include <cstdint>
#include <coroutines/value_promise.hpp>
#include <utility>

namespace
{

    class HandlerGenerator;
    using promise = value_promise<HandlerGenerator>;

    class Awaitable
    {
    public:
        Awaitable(std::optional<int32_t> v) : value_(v) {}
        bool await_ready(void) const noexcept { return false; }
        void await_suspend(std::coroutine_handle<promise> handle) noexcept { handle.promise().value_ = value_; }
        void await_resume(void) const noexcept {}

    private:
        std::optional<int32_t> value_{};
    };

    class HandlerGenerator
    {
    public:
        using promise_type = promise;

        explicit HandlerGenerator(std::coroutine_handle<promise_type> h) : handle_(h)
        {
            printf("HandlerGenerator constructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
        }

        // move constructor used in std::make_unique
        HandlerGenerator(HandlerGenerator &&o) : handle_{std::exchange(o.handle_, {})}
        {
            printf("HandlerGenerator move constructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
        }

        // move assignment operator used in global variable assignment

        ~HandlerGenerator()
        {
            printf("HandlerGenerator destructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
            destroy();
        }

        HandlerGenerator &operator=(HandlerGenerator &&o) = delete;
        HandlerGenerator(const HandlerGenerator &) = delete;
        HandlerGenerator &operator=(const HandlerGenerator &) = delete;

        std::optional<int32_t> get_value(void) const
        {
            auto result = handle_.promise().value_;
            handle_.resume();
            return result;
        }

    private:
        std::coroutine_handle<promise_type> handle_;

        void destroy(void)
        {
            if (handle_)
            {
                handle_.destroy();
                handle_ = {};
            }
        }
    };

    HandlerGenerator catalan_numbers(void)
    {
        int32_t n = 0;
        int32_t c = 1;
        while (true)
        {
            co_await Awaitable{c};
            n++;
            c = c * 2 * (2 * n - 1) / (n + 1);
        }
    }

    std::unique_ptr<HandlerGenerator> handler_ptr{};
}

namespace coroutines
{
    namespace generators
    {
        void awaitable(void)
        {
            printf("Awaitable generator\n");
            handler_ptr = std::make_unique<HandlerGenerator>(catalan_numbers());
            for (size_t i = 0; i < 10; i++)
            {
                printf("Generator awaitable received value: %ld\n", handler_ptr->get_value().value());
            }
        }

    }
}