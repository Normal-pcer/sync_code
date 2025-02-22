#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) {
        i32 a, b, k, l;  std::cin >> a >> b >> k >> l;

        if (l == 0) {
            std::cout << a + b << endl;
            continue;
        }

        auto x_m1 = a / l;
        auto y_m1 = b / l;

        auto base = std::min(k, x_m1 + y_m1);
        auto reduce = base * l;
        auto k_remain = k - base;

        auto x_re = a % l;
        auto y_re = b % l;
        auto x_ex = (x_re * 2) > l;
        auto y_ex = (y_re * 2) > l;

        std::vector<i32> vec;
        if (x_ex)  vec.push_back(2 * x_re - l);
        if (y_ex)  vec.push_back(2 * y_re - l);
        ranges::sort(vec, std::greater{});

        auto take = std::min<i32>(k_remain, vec.size());
        for (auto x: vec | views::take(take))  reduce += x;

        auto ans = std::max(0, a + b - reduce);
        std::cout << ans << endl;
    }
}