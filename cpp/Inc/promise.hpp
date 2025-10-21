#ifndef PROMISE_HPP
#define PROMISE_HPP
#include <coroutine>

template <typename>
class Handler;

class promise;

using handle_t = std::coroutine_handle<promise>;

class promise
{
public:
    Handler<promise> get_return_object(void) noexcept { return Handler<promise>(handle_t::from_promise(*this)); };
    std::suspend_never initial_suspend(void) noexcept { return {}; }
    std::suspend_always final_suspend(void) noexcept { return {}; }
    void return_void(void) {}
    void unhandled_exception(void) { std::terminate(); }
};

#endif // PROMISE_HPP