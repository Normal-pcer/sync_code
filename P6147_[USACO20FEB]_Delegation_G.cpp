/**
 * @link https://www.luogu.com.cn/problem/P6147
 */
#include "./libs/debug_macros.hpp"


#include "./lib_v4.hpp"


#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 考虑如何把一个点分配到一条链上？
 * x 的儿子 - x - x 的另一个儿子
 * x 的儿子 - x - x 的祖先
 * 
 * 维护有多少个链希望连接到当前点上
 * 希望连接：可以从儿子向上延伸，并且长度不足
 * 如果只有一条链，那么选择接着向上延伸
 * 如果有两条链，如果通过当前节点可以成功拼接，那么选择拼接，否则无解。
 * 如果有更多的链，那么无解。
 * 
 * 如何实现代码？
 * 使用 dfs，先获得子树的信息。
 * 至多有一条链能够接着延伸上去，函数返回值表示这条链的有无和长度
 */
namespace Solution_1323743511846362 {

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<std::vector<i32>> graph(N+1);
        for (i32 _ = 0; _ < N - 1; _++) {
            i32 x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        // 按照上述策略分配
        // 如果有向上延伸链，返回长度
        // 否则，返回 0
        // k >= 2
        auto no_solution = false;
        auto dfs = [&](i32 p, i32 prev, i32 k, auto &&self) -> i32 {
            std::multiset<i32> children;  // 所有孩子的返回值
            for (auto x: graph.at(p))  if (x != prev) {
                auto this_child = self(x, p, k, self);
                debug  std::cout << std::format("dfs {} {} {} -> {}", x, p, k, this_child) << std::endl;
                if (no_solution)  return 0;
                if (this_child != k) {
                    children.insert(this_child);
                }
            }

            debug  std::cout << std::format("dfs {} {} {} children: ", p, prev, k) << std::endl;
            debug  for (auto x: children)  std::cout << x << " ";
            debug  std::cout << std::endl;

            // 尝试给 children 配对
            std::vector<i32> extra;
            for (auto it = children.begin(); it != children.end(); ) {
                auto value = *it;
                auto other = k - value;

                it = children.erase(it);  // 擦除自身
                auto it2 = children.find(other);
                if (it2 != children.end()) {
                    if (it == it2)  ++it;  // 此时 it 和 it2 一起失效，跑到下一个
                    children.erase(it2);
                } else {
                    extra.push_back(value);
                }
            }

            if (extra.size() == 0) {
                return 1;  // 作为最底端，向上延伸
            } else if (extra.size() == 1) {
                return extra.back() + 1;  // 接着唯一的未配对向上延伸
            } else {
                // 无解
                no_solution = true;
                return 0;
            }
        };

        std::cout << 1;  // k = 1 必然有解
        for (i32 i = 2; i < N; i++) {
            if ((N - 1) % i != 0)  std::cout << 0;
            else {
                no_solution = false;
                auto final = dfs(1, 0, i, dfs);
                if (no_solution or (final != 1))  std::cout << 0;
                else  std::cout << 1;
            }
        }
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1323743511846362::solve();
    return 0;
}
