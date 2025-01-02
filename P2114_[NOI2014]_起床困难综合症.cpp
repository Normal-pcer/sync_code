/**
 * @link https://www.luogu.com.cn/problem/P2114
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_4496534134520446 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        unsigned int low = 0, high = ~0;

        int N, M;  std::cin >> N >> M;
        for (auto i = 0; i < N; i++) {
            std::string op;
            int x;
            std::cin >> op >> x;
            if (op == "AND")  low &= x, high &= x;
            else if (op == "OR")  low |= x, high |= x;
            else  low ^= x, high ^= x;
        }

        auto ans = 0;
        for (auto i = 0; i < std::numeric_limits<unsigned int>::digits; i++) {
            auto mask = 1U << i;
            if (low & mask)  ans += mask;
            else if (high & mask and (unsigned)M >= mask)  ans += mask;
        }
        std::cout << (unsigned)ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4496534134520446::solve();
    return 0;
}
