/**
 * @link https://www.luogu.com.cn/problem/T587716
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_9068314278056979 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 constexpr maxC = 10;
        std::vector<i64> limits(maxC + 1, infLL);

        i32 N, M;  std::cin >> N >> M;
        struct Word {
            i32 c = 0;
            i64 s = 0;
        };
        std::vector<Word> words(N + 1);
        for (auto &x: words | views::drop(1))  std::cin >> x.c;
        for (auto &x: words | views::drop(1))  std::cin >> x.s;

        for (auto m = M; m --> 0; ) {
            i64 op, l, r, k;  std::cin >> op >> l >> r >> k;
            if (op == 1) {
                for (i32 i = l; i <= r; i++)  words[i].c = k;
            } else if (op == 2) {
                for (i32 i = l; i <= r; i++)  words[i].s += k;
            } else {
                if (l == 1) {
                    limits[r] = k;
                } else {
                    i32 ans = 0;
                    for (i32 i = r; i <= k; i++) {
                        if (words[i].s >= limits[words[i].c])  ans++;
                    }
                    std::cout << ans << endl;
                }
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9068314278056979::solve();
    return 0;
}
