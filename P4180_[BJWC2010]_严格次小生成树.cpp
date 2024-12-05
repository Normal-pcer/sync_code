/**
 * @link https://www.luogu.com.cn/problem/P4180
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1446046906558451 {

    struct Edge {
        int x, y, val;
    };

    struct Node {
        int to, val;
    };

    struct Max2 {
        int first = -inf, second = -inf;

        auto update(int cmp) -> Max2& {
            // std::cout << std::format("first = {}, second = {}, cmp = {}", first, second, cmp) << std::endl;
            if (cmp > first)  second = first, first = cmp;
            else if (first > cmp and cmp > second)  second = cmp;
            assert(first <= -inf or first != second);
            return *this;
        }

        auto update(const Max2 &other) {
            update(other.first), update(other.second);
        }
    };

    struct Jumping: public Max2 {
        int to;

        Jumping(int to = 0):
            Max2({-inf, -inf}), to(to) {}
    };

    int ST;

    std::vector<std::vector<Node>> graph;
    std::vector<int> father, size;
    auto find(int x) {
        if (father.at(x) == x)  return x;
        else  return (father.at(x) = find(father.at(x)));
    }

    auto connect(int x, int y) -> void {
        auto a = find(x), b = find(y);
        if (a == b)  return;
        if (size.at(a) < size.at(b))  std::swap(a, b), std::swap(x, y);

        size.at(a) += size.at(b), father.at(b) = a;
    }

    std::vector<int> depth;
    std::vector<std::vector<Jumping>> F;  // 倍增

    void dfs_pre(int p, Node prev) {
        depth.at(p) = depth.at(prev.to) + 1;

        F.at(p).at(0) = {prev.to}, F.at(p).at(0).update(prev.val);
        for (auto i: range(1, ST)) {
            auto to = F[F[p][i-1].to][i-1].to;
            Jumping res(to);
            res.update(F[p][i-1]), res.update(F[F[p][i-1].to][i-1]);
            F.at(p).at(i) = res;
        }

        for (auto dest: graph.at(p))  if (dest.to != prev.to) {
            dfs_pre(dest.to, {p, dest.val});
        }
    }

    auto findMax(int x, int y) {
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto distance = depth.at(x) - depth.at(y);

        Max2 max;

        for (auto i: range(ST)) {
            if (distance & (1<<i)) {
                max.update(F.at(x).at(i));
                x = F.at(x).at(i).to;
            }
        }

        if (x == y)  return max;

        for (auto i: range(ST) | views::reverse) {
            if (F.at(x).at(i).to != F.at(y).at(i).to) {
                max.update(F[x][i]), max.update(F[y][i]);
                x = F[x][i].to, y = F[y][i].to;
            }
        }
        max.update(F.at(x).at(0)), max.update(F.at(y).at(0));

        return max;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;
        std::cin >> N >> M;
 
        std::vector<Edge> edges(M);
        for (auto &[x, y, val]: edges)  std::cin >> x >> y >> val;

        ranges::sort(edges, lam(x, y, x.val < y.val));
        // 初始化并查集
        father.resize(N+1), ranges::copy(range(N+1), father.begin());
        size.resize(N+1), ranges::fill(size, 1);
        std::vector<Edge> selected, other_edges;
        for (auto [x, y, val]: edges) {
            if (find(x) != find(y))  connect(x, y), selected.emplace_back(x, y, val);
            else  other_edges.emplace_back(x, y, val);
        }

        graph.resize(N+1);
        auto sum = 0LL;
        for (auto [x, y, val]: selected) {
            debug std::cout << std::format("New Edge {{{}, {}, {}}}", x, y, val) << std::endl;
            graph.at(x).emplace_back(y, val);
            graph.at(y).emplace_back(x, val);
            sum += val;
        }

        ST = std::__lg(N) + 1;
        depth.resize(N+1);
        std::fill_n(std::back_inserter(F), N+1, std::vector<Jumping>(ST));
        dfs_pre(1, {0, -inf});

        debug {
            for (auto i: depth | std::views::drop(1))  std::cout << i << ' ';
            std::cout << std::endl;
            for (auto &row: F) {
                for (auto i: row) {
                    std::cout << std::format("({}, {}, {}) ", i.to, i.first, i.second);
                }
                std::cout << std::endl;
            }
        }

        auto delta_min = inf;
        for (auto [x, y, val]: other_edges) {
            auto [max, max_sec] = findMax(x, y);
            auto used = max;
            if (max == val) {
                used = max_sec;
            }
            debug std::cout << std::format("delta at {}, {}, {} = {} (max = {}, sec = {})\n", x, y, val, val - used, max, max_sec);
            chkMin(delta_min, val - used);
        }

        std::cout << sum + delta_min << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1446046906558451::solve();
    return 0;
}
