/**
 * @link https://www.luogu.com.cn/problem/P11477
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

/**
 * f(y) 表示 y 子树上所有点的 点权乘以到 y 的距离 求和
 * 考虑怎么计算给定 y 的 f(y)
 * y 子树上的节点到 y 的距离只和深度有关
 * 一个点的点权乘以它的深度可能比较重要
 * f(y) = sigma val[i] * (d[i] - d[y])
 *      = sigma val[i] * d[i] - sigma val[i] * d[y]
 * 只和每一个点的 val[i] * d[i] 和 val[i] 有关
 * 
 * 题中给定了如下操作（保证 y 为 x 的祖先）：
 * 1. x 的儿子接到 x 的父亲上
 * 2. 删除 x
 * 3. 把 x 插到 y 和它（子树包含 x 的一个儿子）的边上
 * 4. 计算最终的 f(y)
 * 这个过程会对哪些点造成什么影响？
 * 记 son[y] 是 y 的儿子，并且 son[y] 是 x 的祖先
 * son[y] 及其子树，上面新增了一个节点，深度 +1
 * x 的子树（不包含 x），自身向上了一层，但是附着的节点深度 +1，整体来看深度不变
 * x 本身，深度变为 y 的深度 +1
 * 
 * 维护每个点的子树上的 val[i] * d[i] 和 val[i] 之和
 * 考虑增量即可
 * 寻找 son[y] 可以使用倍增法
 */
namespace Solution_9947765739272423 {
    class Tree {
        static constexpr const int MaxLog = 20;
        int N;
        std::vector<int> val;
        std::vector<std::vector<int>> graph;
        std::vector<int> depth;
        std::vector<ll> val_sum;
        std::vector<ll> val_times_depth_sum;
        std::vector<std::array<int, MaxLog>> F;  // 倍增
    public:
        auto dfs(int p, int fa) -> void {
            F[p][0] = fa;
            depth[p] = depth[fa] + 1;
            val_sum[p] = val[p];
            val_times_depth_sum[p] = static_cast<ll>(val[p]) * depth[p];
            for (auto x: graph[p])  if (x != fa) {
                dfs(x, p);
                val_sum[p] += val_sum[x];
                val_times_depth_sum[p] += val_times_depth_sum[x];
            }
        }

        auto query(int x, int y) -> ll {
            // 计算没有修改时的 f(y)
            // f(y) = sigma val[i] * d[i] - sigma val[i] * d[y]
            auto f = val_times_depth_sum[y] - depth[y] * val_sum[y];
            // 进行修改
            // 先找到 son[y]
            auto dis = depth[x] - depth[y] - 1;
            auto son_y = x;
            for (auto j = 0; j < MaxLog; j++) {
                if (dis & (1U << j))  son_y = F[son_y][j];
            }
            assert(F[son_y][0] == y);

            // son[y] 及其子树的深度都需要 +1
            // 整体 f(y) 加了 sigma val[son[y]]
            f += val_sum[son_y];
            // x 及其子树多加了
            f -= val_sum[x];
            // 特别地，现在计算了 x 对应的贡献是 val[x] * depth[x]（原始深度）
            // 实际上，x 的新深度为 depth[y] + 1
            f -= static_cast<ll>(val[x]) * depth[x];
            f += static_cast<ll>(val[x]) * (depth[y] + 1);
            return f;
        }
        Tree(int N): N(N), val(N+1), graph(N+1), depth(N+1), val_sum(N+1), val_times_depth_sum(N+1), F(N+1) {}

        friend auto operator>> (std::istream& is, Tree& tr) -> std::istream& {
            auto N = tr.N;
            for (auto &x: tr.val | views::drop(1))  is >> x;
            for (auto i = 2; i <= N; i++) {
                int fa;  is >> fa;
                tr.graph[fa].push_back(i);
                tr.graph[i].push_back(fa);
            }

            tr.dfs(1, 0);
            for (auto j = 1; (1 << j) <= N; j++) {
                for (auto i = 1; i <= N; i++) {
                    tr.F[i][j] = tr.F[tr.F[i][j-1]][j-1];
                }
            }

            return is;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        Tree tree{N};  std::cin >> tree;

        while (Q --> 0) {
            int x, y;  std::cin >> x >> y;
            std::cout << tree.query(x, y) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9947765739272423::solve();
    return 0;
}
