
#include <optional>
#include <coroutine>
#include <cstdio>
#include <utility>
#include <memory>
#include <coroutines/generators/fibonacci_co_yield.hpp>

namespace
{
    struct promise;

    struct HandlerGenerator
    {
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

        std::optional<int32_t> get_value(void) const;

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

    struct promise
    {
        HandlerGenerator get_return_object() { return HandlerGenerator{std::coroutine_handle<promise>::from_promise(*this)}; }
        std::suspend_never initial_suspend(void) noexcept { return {}; }
        std::suspend_always final_suspend(void) noexcept { return {}; }
        void return_void(void) {}
        void unhandled_exception(void) { std::terminate(); }

        std::suspend_always yield_value(int32_t v) noexcept
        {
            value_ = v;
            return {};
        }

        std::optional<int32_t> value_{};
    };

    std::optional<int32_t> HandlerGenerator::get_value(void) const
    {
        auto result = handle_.promise().value_;
        handle_.resume();
        return result;
    }

    HandlerGenerator fibonacci_numbers(void)
    {
        int32_t i1 = 1;
        int32_t i2 = 1;
        while (true)
        {
            co_yield i1;
            i1 = std::exchange(i2, i1 + i2);
        }
    }

    std::unique_ptr<HandlerGenerator> handler_ptr{};
}

namespace coroutines
{
    namespace generators
    {
        void fibonacci_co_yield(void)
        {
            printf("Fibonacci co_yield generator\n");
            handler_ptr = std::make_unique<HandlerGenerator>(fibonacci_numbers());
            for (size_t i = 0; i < 17; i++)
            {
                printf("Generator co_yeild received value: %ld\n", handler_ptr->get_value().value());
            }
        }

    }
}