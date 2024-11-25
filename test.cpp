/**
 * 
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)
#define main() main(int argc, char const *argv[])
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{}
#if __cplusplus > 201703LL
namespace rgs {  using namespace std::ranges; using namespace std::views;  }
#endif

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1175088154992405 {

    const int _ST = 17;
    const int _N = 1e5+5;

    std::vector<std::vector<int>> graph;
    std::vector<int> depth;
    int F[_N][_ST];

    int LCA(int x, int y) {
        if (depth[x] < depth[y])  std::swap(x, y);

        auto distance = depth[x] - depth[y];
        for (auto i = 0; i < _ST; i++) {
            if (distance & (1<<i)) {
                distance ^= 1<<i;
                x = F[x][i];
            }
        }

        if (x == y)  return x;
        for (signed i = _ST - 1; i >= 0; i--) {
            if (F[x][i] != F[y][i]) {
                x = F[x][i], y = F[y][i];
            }
        }
        return F[x][0];
    }

    void dfs(int p, int prev) {
        depth[p] = depth[prev] + 1;
        F[p][0] = prev;
        for (auto i = 1; i < _ST; i++) {
            F[p][i] = F[F[p][i-1]][i-1];
        }
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
        }
    }

    void solve() {

        int N, K;  io >> N >> K;
        graph.resize(N+1);

        std::vector<char> flag(N+1);
        for (auto i = 0; i < N-1; i++) {
            int x, y;  io >> x >> y;
            graph[x].push_back(y), graph[y].push_back(x);
            flag[y] = '\1';
        }

        auto find = std::find(flag.begin()+1, flag.end(), '\0');
        assert(find != flag.end());
        auto root = find - flag.begin();

        depth.resize(N+1);
        dfs(root, 0);

        for (auto i = 0; i < K; i++) {
            int x, y;  io >> x >> y;
            io << LCA(x, y) << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1175088154992405::solve();
    return 0;
}