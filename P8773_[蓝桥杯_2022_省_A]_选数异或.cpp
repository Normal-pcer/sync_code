/**
 * 
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using namespace lib;

namespace Solution_1036338159763302 {

    int N, M, X;
    const int _N = 1e5+5, _X = 1048576, _LG = 21;

    int a[_N], buc[_X];
    int prev[_N];  // 前面一个能配对的位置
    int F[_N][_LG];

    void solve() {
        io >> N >> M >> X;

        from(i, 1, N) {
            io >> a[i];
        }

        from(i, 1, N) {
            auto cur = a[i];
            auto other = X ^ cur;
            
            prev[i] = buc[other];
            buc[cur] = i;
        }

        from(i, 1, N)  F[i][0] = prev[i];
        auto lg = std::__lg(N);

        from(j, 1, lg) {
            from(l, 1, N) {
                F[l][j] = std::max(F[l][j-1], F[l + (1<<(j-1))][j-1]);
            }
        }

        auto query = [&](int l, int r) {
            auto lg = std::__lg(r - l + 1);
            auto l1 = l, l2 = r - (1<<lg) + 1;
            return std::max(F[l1][lg], F[l2][lg]);
        };

        from(_, 1, M) {
            int x, y;
            io >> x >> y;

            auto max_prev = query(x, y);
            if (max_prev >= x)  io << "yes" << endl;
            else  io << "no" << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1036338159763302::solve();
    return 0;
}