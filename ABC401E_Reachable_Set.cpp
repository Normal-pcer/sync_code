/**
 * @link https://atcoder.jp/contests/abc401/tasks/abc401_e
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_2267377084282644 {
    class DSU {
    public:
        std::vector<i32> parent;
        std::vector<i32> size;
        DSU(i32 n): parent(n + 1), size(n + 1, 1) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        auto find(i32 x) -> i32 {
            if (parent[x] == x) return x;
            else return parent[x] = find(parent[x]);
        }
        auto merge(i32 x, i32 y) -> auto {
            auto a = find(x), b = find(y);
            if (size[a] > size[b]) {
                using std::swap;
                swap(a, b);
#if false
                std::swap(x, y);
#endif  // false
            }
            size[b] += size[a];
            parent[a] = b;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<std::vector<i32>> graph(n + 1);
        for (auto _ = m; _ --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;

            graph[x].push_back(y);
            graph[y].push_back(x);
        }

        DSU dsu{n};
        i32 ans = 0;

        std::vector<char> isDeleted(n + 1);
        for (i32 i = 1; i <= n; i++) {
            if (isDeleted[i]) {
                // 自己被删了
                isDeleted[i] = false;
                ans--;
            }
            for (auto j: graph[i]) {
                if (j < i) {
                    if (dsu.find(i) != dsu.find(j)) {
                        dsu.merge(i, j);
                    }
                } else {
                    if (not isDeleted[j]) {
                        ans++;
                        isDeleted[j] = true;
                    }
                }
            }
            if (dsu.size[dsu.find(1)] == i) {
                std::cout << ans << endl;
            } else {
                std::cout << -1 << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_2267377084282644::solve();
    return 0;
}
