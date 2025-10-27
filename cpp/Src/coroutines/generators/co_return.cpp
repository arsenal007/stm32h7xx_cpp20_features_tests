
#include <optional>
#include <coroutine>
#include <cstdio>
#include <utility>
#include <memory>
#include <coroutines/generators/co_return.hpp>

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
        void return_value(int32_t v) noexcept
        {
            printf("return value\n");
            value_ = v;
        }
        void unhandled_exception(void) { std::terminate(); }

        std::optional<int32_t> value_{};
    };

    std::optional<int32_t> HandlerGenerator::get_value(void) const
    {
        return handle_.promise().value_;
    }

    HandlerGenerator simple_co_return_value(void)
    {
        co_return 17;
    }

    std::unique_ptr<HandlerGenerator> handler_ptr{};
}

namespace coroutines
{
    namespace generators
    {
        void simple_co_return(void)
        {
            printf("simple co_return\n");
            handler_ptr = std::make_unique<HandlerGenerator>(simple_co_return_value());
            printf("Generator co_return received value: %ld\n", handler_ptr->get_value().value());
        }

    }
}