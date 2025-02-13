/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_g
 */
#include "libs/debug_macros.hpp"
#include "lib"
using namespace lib;

using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;

/**
 * x[i] = A * x[i-1] + B
 * x[i] + m = A * (x[i-1] + m)，故 m = B / (A - 1)
 * x[i] + m = pow(A, i) * (x[0] + m)
 * 求 x[i] = G 时的 i
 * 
 * 即 pow(A, i) = K（K 为常数）
 * 令 q = floor(sqrt(p))
 * 预处理出 pow(A, 0), pow(A, q), pow(A, 2*q), pow(A, 3*q), ...
 * 以及 pow(A, 0), pow(A, 1), pow(A, 2), ..., pow(A, q-1)
 */
namespace Solution_1355059899452297 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        i32 P, A, B, S, G;  std::cin >> P >> A >> B >> S >> G;

        // 求解 
        auto log_mod = [&](int x, int s) -> 
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1355059899452297::solve();
    return 0;
}
