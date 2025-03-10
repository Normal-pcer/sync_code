// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    #include "libs/fixed_int.hpp"
    using namespace lib;
    #include "randlib.hpp"
    using namespace Random;

    void generate(std::ostream &out) {
        while (true) {
            i32 constexpr maxN = 10, maxAB = 10, maxC = 5, maxQ = 5;
            struct Obj { i32 a, b, c; };

            i32 N = maxN;
            std::vector<Obj> objs(N);
            for (auto &[a, b, c]: objs) {
                a = randint(1, maxAB * 3 / 10), b = randint(maxAB * 8 / 10, maxAB), c = randint(1, maxC);
                if (a == b)  b++;
                if (a > b)  std::swap(a, b);
            }

            out << N << endl;
            for (auto [a, b, c]: objs)  out << c << " " << a << " " << b << endl;
            auto Q = maxQ;
            out << Q << endl;
            for (auto q = Q; q --> 0; ) {
                auto yes = [&]() -> void {
                    auto copy = objs;
                    ranges::sort(copy, std::greater{}, lam(x, x.b - x.a));
                    auto avaliable_cnt = N;
                    for (uz i = 0; i + 1 < copy.size(); i++) {
                        if (randint(1, 5) == 1)  std::swap(copy[0], copy[1]);
                    }
                    auto choose_cnt = randint(1, std::max<i32>(1, avaliable_cnt));
                    auto min_b = ranges::min(copy | views::take(choose_cnt) | views::transform(lam(x, x.b)));
                    auto max_a = ranges::max(copy | views::take(choose_cnt) | views::transform(lam(x, x.a)));
                    auto m = max_a;
                    auto s = min_b - m;
                    s = std::max(s, 1);

                    auto k = ranges::fold_left(copy | views::take(choose_cnt) | views::transform(lam(x, x.c)), 0, std::plus{});
                    out << m << " " << k << " " << s << endl;
                };
                auto no = [&]() -> void {
                    i32 maxM = 10, maxK = 10, maxS = 10;
                    out << randint(1, maxM) << " " << randint(1, maxK) << " " << randint(1, maxS) << endl;
                };

                if (randint(1, 3) == 1)  no();
                else  yes();
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}