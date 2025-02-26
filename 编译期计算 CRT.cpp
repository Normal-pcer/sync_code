#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"

auto constexpr qpow(i64 a, i64 b, i32 const mod) -> i64 {
    i64 res = 1;
    for (; b; b >>= 1, a = a * a % mod) {
        if (b & 1)  res = res * a % mod;
    }
    return res;
}

template <typename T, T ...args>
struct pack_size;

template <typename T, T x, T ...args>
struct pack_size<T, x, args...> {
    uz static constexpr value = pack_size<T, args...>::value + 1;
};

template <typename T>
struct pack_size<T> { uz static constexpr value = 0; };

template <typename T, T ...args>
uz constexpr pack_size_v = pack_size<T, args...>::value;

template <typename Indices, typename T>
struct tuple_of_n_impl;

template <uz ...Is, typename T>
struct tuple_of_n_impl<std::index_sequence<Is...>, T> {
    using type = std::tuple<decltype(Is, T())...>;
};

template <typename T, std::size_t N>
using tuple_of_n = tuple_of_n_impl<std::make_index_sequence<N>, T>::type;

template <i32 i, i32 N, typename A, typename B>
auto constexpr f_recursion(i64 ans, i64 mod_mul, A const &a, B const &b) -> i64 {
    if constexpr (i == N) {
        return ans;
    } else {
        i32 ai = std::get<i>(a);
        i32 bi = std::get<i>(b);
        i64 m = mod_mul / ai;
        i64 x = qpow(m % ai, ai - 2, ai);
        ans = (ans + m * x % mod_mul * bi % mod_mul) % mod_mul;
        return f_recursion<i + 1, N, A, B>(ans, mod_mul, a, b);
    }
}

template <i32 ...mods, typename ...Ts>
auto constexpr f(Ts ...args) -> i64 {
    uz constexpr nums_count = pack_size_v<i32, mods...>;
    tuple_of_n<i32, nums_count> a_tuple{mods...};  // 模数
    std::tuple<Ts...> b_tuple{args...};

    // static_assert(pack_size_v<i32, mods...> == 4);
    i64 mod_mul = (static_cast<i64>(mods) * ...);
    i64 ans = f_recursion<0, nums_count>(0, mod_mul, a_tuple, b_tuple);
    return ans;
}

int main() {
    std::cout << f<2, 3, 5>(1, 1, 1) << std::endl;
}
