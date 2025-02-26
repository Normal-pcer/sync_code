// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    #include "libs/fixed_int.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());
        std::mt19937_64 random64(std::random_device{}());

        int randint(int x, int y) { return random() % (y - x + 1) + x; }
        int rand_max(int y) { return randint(1, y); }
        long long randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        while (true) {
            i32 const _N = 1e5, _M = 2e5, _K = 4e5;
            // 先随一棵树
            auto N = _N, M = _M, K = _K, T = 1;
            std::vector<i32> prev(N + 1);
            for (i32 i = 2; i <= N; i++) {
                prev[i] = randint(std::max(1, i - 5), i - 1);
            }

            std::vector<std::pair<i32, i32>> edges;
            std::vector<std::set<i32>> graph(N+1);
            for (i32 i = 2; i <= N; i++) {
                edges.push_back({i, prev[i]});
                graph[i].insert(prev[i]);
            }
            for (i32 i = N; i <= M; ) {
                auto p = randint(1, N), q = randint(1, N);
                if (p == q)  continue;
                if (graph[p].contains(q))  continue;
                graph[p].insert(q);
                edges.push_back({p, q});
                i++;
            }
            out << N << " " << M << " " << K << " " << T << endl;
            for (auto [x, y]: edges)  out << x << " " << y << endl;
            for (auto _: range(K))  out << randint(1, N) << " ";
            out << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}