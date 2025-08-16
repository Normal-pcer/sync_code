// Do not expand include
#ifndef INCLUDING_GENERATOR
#include <bits/stdc++.h>
#endif
namespace Generator {
    char constexpr endl = '\n';
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;
#include "libs/range.hpp"
#include "libs/fixed_int.hpp"
    using namespace lib;
#include "randlib.hpp"
    using namespace Random;

#define cout  // 避免误用 cout 输出
    void generate(std::ostream &out) {
        i32 constexpr maxN = 5e4, maxM = 1e5, maxQ = 5e4, maxVal = 1e6;
        auto n = maxN, m = maxM, q = maxQ;

        std::set<std::pair<i32, i32>> s;
        out << n << ' ' << m << ' ' << q << endl;

        for (i32 p = 2; p <= n; ++p) {
            auto prev = randint(1, p - 1);
            s.emplace(prev, p);
            out << prev << ' ' << p << endl;
        }

        while (s.size() < u32(m)) {
            auto x = randint(1, n), y = randint(1, n);
            if (x > y) std::swap(x, y);
            if (x == y) continue;
            if (s.contains({x, y})) continue;
            s.emplace(x, y);
            out << x << ' ' << y << ' ' << randint(1, maxVal) << endl;
        }

        for (i32 _ = 0; _ < q; ++_) {
            auto x = randint(1, n), y = randint(1, n);
            auto k = randint(1, n);
            auto c = randint(0, k - 1);
            out << x << ' ' << y << ' ' << k << ' ' << c << endl;
        }
    }
#undef cout
}

int main() {
    Generator::generate(std::cout);
    return 0;
}