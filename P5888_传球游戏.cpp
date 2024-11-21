/**
 * @link https://www.luogu.com.cn/problem/P5888
 */

#include "lib"
#include "libs/io.hpp"
#include "libs/rollingArray2.hpp"
using namespace lib;

const int mod = 998244353;

// input:
// 

namespace Solution {
    int N, M, K;
    const int _N = 505, _K = 50005;
    int max_mapped = 0;
    std::unordered_map<int, int> unmapped;
    std::unordered_map<int, int> mapped;
    std::unordered_set<int> limits[_N];

    RollingArray2<int[_K<<1], 4> F;
    
    void init() {
        io >> N >> M >> K;
        int a, b;
        upto(_, K) {
            io >> a >> b;
            limits[a].insert(b);
            if (mapped[a] == 0) {
                unmapped[++max_mapped] = a;
                mapped[a] = max_mapped;
            }
            if (mapped[b] == 0) {
                unmapped[++max_mapped] = b;
                mapped[b] = max_mapped;
            }
        }

        if (mapped[1] == 0) {
            unmapped[++max_mapped] = 1;
            mapped[1] = max_mapped;
        }
    }

    void solve() {
        init();

        F[0][mapped[1]] = 1;
        from(i, 1, M) {
            std::memset(F[i], 0, sizeof(F[i]));
            from(j, )
        }
    }
}

int main() {
    initDebug;
    Solution::solve();
    return 0;
}