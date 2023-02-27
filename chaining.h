#pragma once

#include <type_traits>
#include <optional>

namespace metafunc
{

template <typename Optional_T>
struct get_optional_type;

template <typename T>
struct get_optional_type<std::optional<T>>
{
    using type = T;
};

template <typename T>
using get_optional_t = typename get_optional_type<T>::type;

template <typename V>
class Chaining
{
public:
    Chaining() = default;
    ~Chaining() = default;
    Chaining(const Chaining& other) = default;
    Chaining(Chaining&& other) = default;
    Chaining& operator=(const Chaining& other) = default;
    Chaining& operator=(Chaining&& other) = default;

    explicit Chaining(V r) :
        res_(r) { }

    template <typename F>
    Chaining<get_optional_t<std::invoke_result_t<F, V>>> than(F f)
    {
        static_assert(std::is_invocable_v<F, V>, "not a function");
        if(!res_.has_value())
        {
            return Chaining<get_optional_t<std::invoke_result_t<F, V>>>();
        }

        auto val = f(res_.value());
        if(!val.has_value())
        {
            return Chaining<get_optional_t<std::invoke_result_t<F, V>>>();
        }

        return Chaining<get_optional_t<std::invoke_result_t<F, V>>>(val.value());
    }

    template <typename F, class... Args>
    Chaining<get_optional_t<std::invoke_result_t<F, Args...>>> than(F f, Args... a)
    {
        static_assert(std::is_invocable_v<F, Args...>, "not a function");
        if(!res_.has_value())
        {
            return Chaining<get_optional_t<std::invoke_result_t<F, Args...>>>();
        }

        auto val = f(std::forward<Args>(a)...);
        if(!val.has_value())
        {
            return Chaining<get_optional_t<std::invoke_result_t<F, Args...>>>();
        }

        return Chaining<get_optional_t<std::invoke_result_t<F, Args...>>>(val.value());
    }

    const V& result() const
    {
        return res_.value();
    }

protected:
    std::optional<V> res_ = std::nullopt;
};

template <typename F>
static Chaining<get_optional_t<std::invoke_result_t<F>>> tryThis(F f)
{
    static_assert(std::is_invocable_v<F>, "not a function");

    auto val = f();
    if(!val.has_value())
    {
        return Chaining<get_optional_t<std::invoke_result_t<F>>>();
    }

    return Chaining<get_optional_t<std::invoke_result_t<F>>>(val.value());
}
} // namespace metafunc
