/**
 * 
 */

#include "lib"
using namespace lib;


namespace Solution_1322932692923613 {
    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;
        std::cin >> N >> M;
        std::vector<std::vector<int>> graph(N+1);
        std::vector<int> in(N+1);
        from(i, 1, M) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            in.at(y)++;
        }
    }
}


int main() {
    initDebug;
    Solution_1322932692923613::solve();
    return 0;
}