/**
 * @link https://www.luogu.com.cn/problem/P6279
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 如果两个奶牛均「仰慕」颜色相同的奶牛，它们的颜色一定相同。
 * 把相同颜色的奶牛「打包」起来。同一个「包」的所有边又会指向同色的奶牛。
 * 
 * 如果一个点会指向多个点，那么就合并这几个点。
 * 把其中一个点的后继节点都转移到另一个节点上
 */
namespace Solution_3041603084108311 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<std::vector<i32>> graph(N+1);
        for (i32 _ = 0; _ < M; _++) {
            i32 x, y;  std::cin >> x >> y;
            graph[x].push_back(y);
        }

        std::deque<i32> q;
        for (i32 i = 1; i <= N; i++) {
            if (graph[i].size() > 0) {
                q.push_back(i);
            }
        }

        std::vector<i32> fa(N+1);
        std::iota(fa.begin(), fa.end(), 0);

        std::vector<std::vector<i32>> son_of(N+1);
        for (i32 i = 1; i <= N; i++)  son_of[i].push_back(i);

        auto merge = [&](i32 x, i32 y) -> void {
            x = fa[x], y = fa[y];
            if (graph[x].size() < graph[y].size())  std::swap(x, y);  // 启发式合并
            // y 的所有孩子迁移到 x 上
            for (auto next: graph[y])  graph[x].push_back(next);
            for (auto son: son_of[y])  son_of[x].push_back(son), fa[son] = x;
            graph[y].clear(), son_of[y].clear();

            if (graph[x].size() > 1)  q.push_back(x);
        };

        while (not q.empty()) {
            auto x = q.front();  q.pop_front();
            while (graph[x].size() > 1) {
                // 合并后两个点
                auto a = graph[x].back();  graph[x].pop_back();
                auto b = graph[x].back();

                if (fa[a] != fa[b]) {
                    debug  std::cout << "merge " << a << " " << b << std::endl; 
                    merge(a, b);
                }
            }
        }

        std::vector<i32> color(N+1);
        i32 next_c = 1;

        for (i32 i = 1; i <= N; i++) {
            if (fa[i] == i) {
                color[i] = next_c++;
                for (auto s: son_of[i]) {
                    color[s] = color[i];
                }
            }
        }

        for (auto c: color | views::drop(1)) {
            std::cout << c << endl;
        }
    }
}

int main(int qwq123, char const *argv[]) {
    DEBUG_MODE = (qwq123-1) and not strcmp("-d", argv[1]);
    Solution_3041603084108311::solve();
    return 0;
}
