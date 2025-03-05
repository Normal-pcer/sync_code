#include <bits/stdc++.h>
#include "libs/fixed_int.hpp"

// template <typename T>
// concept dereferencable = requires(decltype(*std::declval<T>()));

namespace ranges = std::ranges;

std::mt19937 rng{std::random_device{}()};

template <typename T, typename U>
auto randint(T min, U max) -> decltype(min + max) {
    using ResultType = decltype(min + max);
    if (min > max)  std::swap(min, max);
    std::uniform_int_distribution<ResultType> rid{static_cast<ResultType>(min), static_cast<ResultType>(max)};
    return rid(rng);
}

template <typename T, typename U>
auto randreal(T min, U max) -> decltype(min + max) {
    using ResultType = decltype(min + max);
    if (min > max)  std::swap(min, max);
    std::uniform_real_distribution<ResultType> rrd{static_cast<ResultType>(min), static_cast<ResultType>(max)};
    return rrd(rng);
}

template <typename T, typename U>
auto randrange(T first, U last) -> decltype(first + last) {
    using ResultType = decltype(first + last);
    if constexpr (std::is_integral_v<ResultType>)  return randint(first, last + 1);
    else if constexpr (std::is_floating_point_v<ResultType>)  return randreal(first, last);
    else  static_assert(false, "first + last must be integral or floating point number.");
}

template <typename T>
auto sorted(T x) -> T {
    ranges::sort(x);
    return x;
}

template <typename T>
auto sample(T first, T last, i32 N) -> std::vector<T> {
    auto cnt = last - first;
    assert(N <= cnt);
    if (cnt < N * 2) {
        std::vector<T> tmp(cnt);
        for (i32 i = 0; i < cnt; i++)  tmp[i] = first + i;
        std::vector<T> res(N);
        std::sample(tmp.begin(), tmp.end(), res.begin(), N, rng); 
        std::shuffle(res.begin(), res.end(), rng);
        return res;
    } else {
        std::set<T> s;
        std::vector<T> res(N);
        for (i32 i = 0; i < cnt; i++) {
            auto x = randrange(first, last);
            for (; s.contains(x); x++);
            s.insert(x);
            res[i] = x;
        }
        return res;
    }
}

template <typename T>
auto operator<< (std::ostream &os, std::vector<T> const &vec) -> std::ostream & {
    for (uz i = 0; i != vec.size(); i++) {
        if (i != 0)  os << " ";
        os << vec[i];
    }
    return os;
}

int main() {
    for (i32 i = 0; i < 10; i++) {
        std::cout << sample(1, 100, 35) << std::endl;
    }
}
