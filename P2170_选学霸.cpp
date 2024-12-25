/**
 * @link https://www.luogu.com.cn/problem/P2170
 */

#include "./libs/debug_log.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1463633277684438 {
    class UnionFind {
        std::vector<int> fa;
        std::vector<int> size;
    public:
        UnionFind(int N): fa(N+1), size(N+1) {
            std::iota(fa.begin(), fa.end(), 0);
            ranges::fill(size, 1);
        }
        int find(int x) {
            if (fa[x] == x)  return x;
            else  return (fa[x] = find(fa[x]));
        }
        void combine(int x, int y) {
            auto a = find(x), b = find(y);
            if (size[a] < size[b])  std::swap(x, y), std::swap(a, b);
            fa[b] = fa[a], size[a] += size[b];
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        UnionFind uf(N+1);
        std::vector<std::pair<int, int>> pairs(K);
        for (auto &[x, y]: pairs)  std::cin >> x >> y, uf.combine(x, y);

        std::vector<std::vector<int>> groups(N+1);
        for (auto i: range(1, N+1))  groups.at(uf.find(i)).push_back(i);

        std::vector<int> items {0};
        for (auto &group: groups) {
            if (not group.empty()) {
                items.push_back(group.size());
            }
        }

        const int _M = 4e4+8;  std::bitset<_M> F;
        F[0] = true;
        for (auto i: range<size_t>(1, items.size())) {
            F |= F << items[i];
            debug  std::cout << std::bitset<16>(F.to_ullong()) << std::endl;
        }
        
        auto ans = [&]() {
            for (auto diff: range(0, M+1)) {
                auto x = M - diff, y = M + diff;
                if (F[x])  return x;
                if (F[y])  return y;
            }
            return 0;
        }();
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1463633277684438::solve();
    return 0;
}
