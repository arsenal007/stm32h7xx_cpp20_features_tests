#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <coroutine>

template <typename promise>
class Handler
{
public:
    using promise_type = promise;

    explicit Handler(std::coroutine_handle<promise_type> h) : handle_(h) {}

    ~Handler()
    {
        if (handle_)
            handle_.destroy();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

#endif // HANDLER_HPP