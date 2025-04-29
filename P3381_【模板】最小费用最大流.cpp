/**
 * @link https://www.luogu.com.cn/problem/P3381
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_1965071402445387 {
    class Flow {
    public:
        struct EdgeNode {
            i32 to = 0;
            i32 next = 0;
            i64 flow = 0;
        };
        struct Result {
            i32 flow;
            i32 cost;
        };
        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;
        
        Flow(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            edges.emplace_back(v, firstEdge[u], val);
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.emplace_back(u, firstEdge[v], 0);
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        auto dinic(i32 s, i32 t) -> Result {
            
        }
    };
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_1965071402445387::solve();
    return 0;
}
