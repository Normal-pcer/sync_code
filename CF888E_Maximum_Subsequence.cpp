/**
 * 
 */

#include "lib"
#include "libs/io.hpp"
using namespace lib;


namespace Solution_4870535850952584 {

    const int _N = 36;
    int a[_N];

    std::set<int> res1, res2;

    template <typename T>
    void dfs(int p, int limit, ll sum, T&& res) {
        if (p > limit) {
            res.insert(sum);
            return;
        }
        dfs(p+1, limit, sum, res);
        dfs(p+1, limit, sum + a[p], res);
    }

    void solve() {
        
    }
}


int main() {
    initDebug;
    Solution_4870535850952584::solve();
    return 0;
}