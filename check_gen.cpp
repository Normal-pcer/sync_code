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
        i32 constexpr _T = 50000;
        i32 T = _T;
        out << 0 << " " << T << endl;
        while (T--)
        while (true) {
            i32 constexpr _N = 8, _V = 20;

            auto N = _N;
            auto ab = sorted(sample(1, _V, N * 2));
            for (i32 i = 0; i < N - 1; i++) {
                if (randint(1, 2) == 1)  std::swap(ab[i * 2 + 1], ab[i * 2 + 2]);
            }
            std::vector<i32> a(N), b(N);
            i64 all_t = 0;
            for (i32 i = 0; i < N; i++) {
                a[i] = ab[i * 2];
                b[i] = ab[i * 2 + 1];
                all_t += b[i] - a[i];
            }
            all_t += randint(-1, 1);

            out << N << endl;
            for (i32 i = 0; i < N; i++) {
                out << a[i] << " " << b[i] << " " << all_t << endl;
            }
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}