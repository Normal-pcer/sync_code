/**
 * 
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;


namespace Solution_3904170373422103 {
    
    const int _N = 2e5+5;
    int N, M;

    std::array<int, _N> F;
    std::array<std::vector<int>, _N> graph;

    auto find(int x) -> int {
        if (x == F[x])  return x;
        else  return (F[x] = find(F[x]));
    }

    auto connect(int x, int y) -> void {
        auto a = find(x), b = find(y);
        F[a] = b;
    }
    
    void solve() {
        io >> N >> M;

        std::vector<std::pair<int, int>> edges;  
        for (auto _: std::views::iota(0, M)) {
            int x, y;
            io >> x >> y;
            edges.push_back({x, y});
        }
        std::vector<int> colors(1);
        for (auto _: std::views::iota(0, N)) {
            int color;
            io >> color;
            colors.push_back(color);
        }

        for (auto i: std::views::iota(1, N+1)) {
            F[i] = i;
        }
        for (auto [x, y]: edges) {
            if (colors.at(x) != colors.at(y))  connect(x, y);
        }

        for (auto [x, y]: edges) {
            if (colors.at(x) == colors.at(y) and find(x) == find(y)) {
                io << "Yes" << endl;
                goto egg;
            }
        }
        io << "No" << endl;
    egg:
        ;
    }
}


int main() {
    initDebug;
    Solution_3904170373422103::solve();
    return 0;
}