#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <coroutine>
#include <cstdio>
#include <utility>

template <typename promise>
class Handler
{
public:
    using promise_type = promise;

    explicit Handler(void) : handle_{}
    {
    }

    explicit Handler(std::coroutine_handle<promise_type> h) : handle_(h)
    {
        printf("Handler constructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
    }

    // move constructor used in std::make_unique
    Handler(Handler &&o) : handle_{std::exchange(o.handle_, {})}
    {
        printf("Handler move constructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
    }

    // move assignment operator used in global variable assignment
    Handler &operator=(Handler &&o) noexcept
    {
        printf("Handler move assignment operator called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
        destroy();
        handle_ = std::exchange(o.handle_, {});
        return *this;
    };

    ~Handler()
    {
        printf("Handler destructor called 0x%08X, coroutine frame: 0x%08X\n", (unsigned int)this, (unsigned int)handle_.address());
        destroy();
    }

    Handler(const Handler &) = delete;
    Handler &operator=(const Handler &) = delete;

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

#endif // HANDLER_HPP