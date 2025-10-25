#ifndef VALUE_PROMISE_HPP
#define VALUE_PROMISE_HPP

#include <coroutine>
#include <optional>

template <typename HandlerType>
class value_promise
{
    using promise = value_promise<HandlerType>;
    using handle_t = std::coroutine_handle<promise>;

public:
    HandlerType get_return_object() { return HandlerType{handle_t::from_promise(*this)}; }
    std::suspend_never initial_suspend(void) noexcept { return {}; }
    std::suspend_always final_suspend(void) noexcept { return {}; }
    void return_void(void) {}
    void unhandled_exception(void) { std::terminate(); }

    std::optional<int> value_{};
};

#endif // VALUE_PROMISE_HPP