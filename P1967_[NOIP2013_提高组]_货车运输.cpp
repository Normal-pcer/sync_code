/**
 * @link https://www.luogu.com.cn/problem/P1967
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6817006028361799 {

    struct Edge {
        int x, y, val;
    };

    struct Node {
        int to, val;
    };

    std::vector<int> father, size;
    auto find(int x) {
        if (x == father.at(x))  return x;
        else  return (father.at(x) = find(father.at(x)));
    }

    void connect(int x, int y) {
        auto a = find(x), b = find(y);
        if (a == b)  return;
        if (size.at(a) < size.at(b))  return connect(y, x);
        size.at(a) += size.at(b), father.at(b) = a;
    }

    std::vector<std::vector<Node>> graph;  // 生成树

    std::vector<int> depth;
    std::vector<std::vector<Node>> F;  // 从 i 跳 1<<j 步
    int ST;  // 倍增步数
    std::vector<char> vis;
    void dfs_pre(int p, Node prev) {
        vis.at(p) = true;
        depth.at(p) = depth.at(prev.to) + 1;

        F.at(p).at(0) = prev;
        for (auto i: range(1, ST)) {
            auto to = F.at(F.at(p).at(i-1).to).at(i-1).to;
            auto val = std::min(F[p][i-1].val, F[F[p][i-1].to][i-1].val);
            F.at(p).at(i) = {to, val};
        }

        for (auto dest: graph.at(p)) {
            if (prev.to != dest.to) {
                dfs_pre(dest.to, {p, dest.val});
            }
        }
    }

    // 查询最小限制
    auto query(int x, int y) {
        auto min = inf;
        if (find(x) != find(y))  return -inf;
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto distance = depth.at(x) - depth.at(y);

        for (auto i: range(ST)) {
            if (distance & (1<<i)) {
                chkMin(min, F[x][i].val);
                x = F.at(x).at(i).to;
            }
        }

        if (x == y)  return min;

        for (auto i: range(ST) | views::reverse) {
            if (F.at(x).at(i).to != F.at(y).at(i).to) {
                chkMin(min, F[x][i].val), chkMin(min, F[y][i].val);
                x = F[x][i].to, y = F[y][i].to;
            }
        }

        return std::min({min, F.at(x).at(0).val, F.at(y).at(0).val});
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;
        std::cin >> N >> M;
        std::vector<Edge> edges(M);
        for (auto &[x, y, val]: edges)  std::cin >> x >> y >> val;
        ranges::sort(edges, lam(x, y, x.val > y.val));

        std::vector<Edge> edges_new;  // 生成树的边
        // 并查集初始化
        father.resize(N+1), ranges::copy(range(N+1), father.begin());
        size.resize(N+1), ranges::fill(size, 1);

        // 从大到小连边
        for (auto [x, y, val]: edges) {
            if (find(x) != find(y)) {
                edges_new.push_back({x, y, val});
                connect(x, y);
            }
        }

        // 新的图
        graph.resize(N+1);
        for (auto [x, y, val]: edges_new) {
            debug std::cout << std::format("New edge: {{{}, {}, {}}}", x, y, val) << std::endl;
            graph.at(x).push_back({y, val});
            graph.at(y).push_back({x, val});
        }

        // 在图上初始化倍增和深度
        depth.resize(N+1);
        ST = std::__lg(N) + 1;
        std::fill_n(std::back_inserter(F), N+1, std::vector<Node>(ST, {0, -inf}));
        vis.resize(N+1);
        for (auto i: range(1, N+1)) {
            if (not vis.at(i))  dfs_pre(i, {0, -inf});
        }

        debug {
            std::cout << "Depth ";
            for (auto i: depth | views::drop(1)) {
                std::cout << i << ' ';
            }
            std::cout << std::endl;
        }

        debug for (auto const &row: F) {
            for (auto const &i: row) {
                std::cout << std::format("({}, {})", i.to, i.val) << ' ';
            }
            std::cout << std::endl;
        }

        int Q;  std::cin >> Q;
        for (auto _: range(Q)) {
            int x, y;  std::cin >> x >> y;
            auto res = query(x, y);
            if (res <= 0)  std::cout << -1 << std::endl;
            else  std::cout << res << std::endl;
        }
    }
}

int main() {
    initDebug;
    Solution_6817006028361799::solve();
    return 0;
}
