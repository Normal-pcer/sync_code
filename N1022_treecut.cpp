/**
 * 
 */

#include "lib"
#include "libs/all.hpp"
using namespace lib;

// input:
// 

namespace Solution {

    int N;
    const int _N = 1e4+5;
    std::vector<int> conn[_N];
    int F[_N];
    
    void init() {
        io >> N;
        upto(_, N-1) {
            int x, y;
            io >> x >> y;
            conn[x].push_back(y);
            conn[y].push_back(x);
        }
    }

    int dfs(int p, int prev) {
        static int C[_N];
        if (C[p])  return C[p];
        int res = 0;
        int cnt = 1;
        for (auto &dest: conn[p]) {
            int cur = dfs(dest, p);
            chkMax(res, cur);
            cnt += cur;
        }
        F[p] = res;
        C[p] = cnt;
        return cnt;
    }

    void solve() {
        init();
        dfs(1, 0);
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}