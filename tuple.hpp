#include <cstddef>
#include <utility>

template <std::size_t _index, typename T>
class _tuple_impl {
public:
    _tuple_impl(T const &v) { val = v; }
    _tuple_impl(T &&v) { val = std::move(v); }
    T &get() { return val; }
private:
    T val;
};

template <std::size_t _index, typename... types>
class _tuple_recurr_base {};

template <std::size_t _index, typename L, typename... types>
class _tuple_recurr_base<_index, L, types...> : public _tuple_impl<_index, L>,
                                                public _tuple_recurr_base<_index + 1, types...> {
public:
    template <typename CL, typename... CArgs>
    _tuple_recurr_base(CL &&arg, CArgs &&... args) : _tuple_impl<_index, CL>(std::forward<CL>(arg)),
                                                     _tuple_recurr_base<_index + 1, types...>(std::forward<CArgs>(args)...)
    {}
};

template <typename L, typename... types>
class tuple : public _tuple_recurr_base<0, L, types...> {
public:
    template <typename... CArgs>
    tuple(CArgs &&... args) : _tuple_recurr_base<0, L, types...>(std::forward<CArgs>(args)...) {}

    template <typename... Args>
    friend bool operator==(tuple<Args...> &t1, tuple<Args...> &t2);
};

template <typename... CArgs>
tuple(CArgs... args)->tuple<CArgs...>;

template <std::size_t index, typename L, typename... Args>
struct extract_type_at {
    using type = typename extract_type_at<index - 1, Args...>::type;
};

template <typename L, typename... Args>
struct extract_type_at<0, L, Args...> {
    using type = L;
};

template <std::size_t index, typename... Args>
auto &get(tuple<Args...> &t) {
    return (static_cast<_tuple_impl<index, typename extract_type_at<index, Args...>::type> &>(t)).get();
}

template <std::size_t index, typename... Args>
bool compare_tuple(tuple<Args...> &t1, tuple<Args...> &t2) {
    if constexpr (index == 0) {
        return get<0>(t1) == get<0>(t2);
    } else {
        return get<index>(t1) == get<index>(t2) && compare_tuple<index - 1>(t1, t2);
    }
}

template <typename... Args>
bool operator==(tuple<Args...> &t1, tuple<Args...> &t2) {
    return compare_tuple<sizeof...(Args) - 1>(t1, t2);
}