/**
 * @link https://www.luogu.com.cn/problem/AT_abc267_f
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1525232159428069 {

    int N;
    int ST;  // 最大倍增步数（不取）
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> F;  // 倍增，从 i 开始跳 (1<<j) 次
    std::vector<int> depth;

    std::pair<int, int> diameter;  // 直径的两个端点
    int phi;  // 直径长度

    // 预处理深度和倍增
    auto dfs_pre(int p, int prev) -> void {
        depth.at(p) = depth.at(prev) + 1;

        F.at(p).at(0) = prev;
        for (auto i: range(1, ST))  F.at(p).at(i) = F.at(F.at(p).at(i-1)).at(i-1);

        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_pre(dest, p);
        }
    }

    // 求 LCA，并判断是否位于同一条链上
    struct LCA_ret {
        int ancestor;
        bool chain;
        int dis_x;
        int dis_y;
    };
    auto _LCA(int x, int y) -> LCA_ret {
        assert(depth.at(x) >= depth.at(y));
        auto distance = depth.at(x) - depth.at(y);

        for (auto i: range(ST)) {
            if (distance & (1<<i))  x = F.at(x).at(i);
        }

        if (x == y)  return {x, true, distance, 0};

        auto dx = 0, dy = 0;
        for (auto i: range(0, ST) | views::reverse) {
            if (F.at(x).at(i) != F.at(y).at(i)) {
                x = F[x][i], y = F[y][i];
                dx += 1<<i, dy += 1<<i;
            }
        }

        return {F.at(x).at(0), false, dx, dy};
    }

    auto LCA(int x, int y) -> LCA_ret {
        bool swapped = false;
        if (depth.at(x) < depth.at(y))  std::swap(x, y), swapped = true;
        auto [ancestor, chain, dx, dy] = _LCA(x, y);
        if (not swapped)  return {ancestor, chain, dx, dy};
        else  return {ancestor, chain, dy, dx};
    }

    auto distance(int x, int y) -> int {
        auto res = LCA(x, y);
        return res.dis_x + res.dis_y;
    }

    auto jump(int origin, int step) -> int {
        for (auto i: range(ST)) {
            if (step & (1<<i))  origin = F.at(origin).at(i);
        }
        return origin;
    }

    struct HalfChain {
        int length;
        int leaf;

        std::weak_ordering operator<=> (const HalfChain &other) {
            return length <=> other.length;
        }
    };

    // 返回最长的半链，并且更新全链的最大值
    auto dfs_diameter(int p, int prev) -> HalfChain {
        std::vector<HalfChain> lens;
        for (auto dest: graph.at(p))  if (dest != prev) {
            auto [len, pnt] = dfs_diameter(dest, p);
            lens.push_back({len, pnt});
        }
        if (lens.empty())  return {1, p};
        // 在所有半链中选择最长的
        auto max_ptr = std::max_element(lens.begin(), lens.end());
        auto max = *max_ptr;  lens.erase(max_ptr);
        if (lens.empty()) {
            // 只能继续向下连
            auto new_len = max.length + 1;
            if (new_len > phi)  phi = new_len, diameter = {p, max.leaf};
            return {new_len, max.leaf};
        }
        auto sec_ptr = std::max_element(lens.begin(), lens.end());
        auto sec = *sec_ptr;

        // 拼接最长的两条半链
        auto new_len = max.length + sec.length + 1;
        if (new_len > phi)  phi = new_len, diameter = {max.leaf, sec.leaf};
        return {max.length + 1, max.leaf};  // 返回半链
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(0), std::cout.tie(0);

        std::cin >> N;
        ST = std::__lg(N) + 1;
        graph.resize(N+1), depth.resize(N+1);
        std::fill_n(std::back_inserter(F), N+1, std::vector<int>(ST));

        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        dfs_pre(1, 0);
        dfs_diameter(1, 0);
        
        debug {
            std::cout << diameter.first << ' ' << diameter.second << ' ' << phi << std::endl;
        }
    

        int Q;  std::cin >> Q;
        for (auto _: range(Q)) {
            int U, K;  std::cin >> U >> K;
            auto [x, y] = diameter;
            auto dx = distance(U, x), dy = distance(U, y);
            if (dx < dy)  std::swap(dx, dy), std::swap(x, y);

            debug std::cout << "x, y " << x << ' ' << y << std::endl;
            debug std::cout << "dx, dy " << dx << ' ' << dy << std::endl;

            if (dx < K) {
                std::cout << -1 << std::endl;
                continue;
            }
            auto [ancestor, chain, left, right] = LCA(U, x);
            if (left >= K) {
                // 从左边跳
                auto dest = jump(U, K);
                std::cout << dest << std::endl;
            } else {
                // 从右边跳
                auto tot = left + right;
                auto remain = tot - K;
                auto dest = jump(x, remain);
                std::cout << dest << std::endl;
            }
        }
    }
}

int main() {
    initDebug;
    Solution_1525232159428069::solve();
    return 0;
}
