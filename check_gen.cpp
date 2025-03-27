// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
#include "lib"
#include "libs/range.hpp"
#include "libs/fixed_int.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 rng{std::random_device{}()};

        template <typename T, typename U>
        auto randint(T min, U max) -> decltype(min + max) {
            using ResultType = decltype(min + max);
            ResultType min_ = min, max_ = max;
            if (min_ > max_)  std::swap(min_, max_);
            std::uniform_int_distribution<ResultType> rid{static_cast<ResultType>(min_), static_cast<ResultType>(max_)};
            return rid(rng);
        }

        template <typename T, typename U>
        auto randreal(T min, U max) -> decltype(min + max) {
            using ResultType = decltype(min + max);
            ResultType min_ = min, max_ = max;
            if (min_ > max_)  std::swap(min_, max_);
            std::uniform_real_distribution<ResultType> rrd{static_cast<ResultType>(min_), static_cast<ResultType>(max_)};
            return rrd(rng);
        }

        template <typename T, typename U>
        auto randrange(T first, U last) -> decltype(first + last) {
            using ResultType = decltype(first + last);
            assert(static_cast<ResultType>(first) != static_cast<ResultType>(last));
            if constexpr (std::is_integral_v<ResultType>)  return randint(first, last - 1);
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
                for (i32 i = 0; i < N; i++) {
                    auto x = randrange(first, last);
                    for (; s.contains(x); x++) {
                        if (x == last)  x = first;
                    }
                    s.insert(x);
                    res[i] = x;
                }
                return res;
            }
        }
        template <typename T>
        auto sample(T &&range, uz N) -> auto {
            auto cnt = range.size();
            using ResultType = std::remove_reference_t<decltype(*range.begin())>;
            assert(N <= cnt);
            if (cnt < N * 2) {
                std::vector<ResultType> res(N);
                std::sample(range.begin(), range.end(), res.begin(), N, rng); 
                std::shuffle(res.begin(), res.end(), rng);
                return res;
            } else {
                std::set<ResultType> s;
                std::vector<ResultType> res(N);
                for (uz i = 0; i < N; i++) {
                    auto x = randrange(0, cnt);
                    for (; s.contains(x); x++);
                    s.insert(range[x]);
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

    }
    using namespace Random;

    void generate(std::ostream &out) {
        while (true) {
            i32 constexpr maxN = 800, maxQ = 800;
            auto N = maxN, Q = maxQ;
            out << N << " " << Q << endl;
            for (auto _: range(N)) out << randint(0, 2) << endl;

            for (auto q = Q; q --> 0; ) {
                auto op = randint(1, 2), l = randint(1, N - 1), r = randint(l, N);
                if (op == 1) out << op << " " << l << " " << r << endl;
                else {
                    out << op << " " << l << " " << r << " " << randint(0, 2) << " " << randint(0, 2) << " " << randint(0, 2) << endl;
                }
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}