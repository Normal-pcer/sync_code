/**
 * @link https://www.luogu.com.cn/problem/P3812
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_7203508507493624 {
    auto constexpr highbit(u64 x) -> u64 {
        auto clz = __builtin_clzll(x);
        return static_cast<u64>(1) << (64 - clz - 1);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i64> vec(N);
        for (auto &x: vec)  std::cin >> x;

        
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7203508507493624::solve();
    return 0;
}
