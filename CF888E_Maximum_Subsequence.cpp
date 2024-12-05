/**
 * 
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using namespace lib;

namespace Solution_4870535850952584 {

    const int _N = 36;
    int N, M;
    int a[_N];

    std::set<int> res1, res2;

    template <typename T>
    void dfs(int p, int limit, ll sum, T&& res) {
        if (p > limit) {
            return;
        }
        res.insert(sum % M);
        dfs(p+1, limit, sum, res);
        dfs(p+1, limit, sum + a[p], res);
    }

    void solve() {
        io >> N >> M;
        from(i, 0, N-1)  io >> a[i];

        auto mid = N >> 1;
        dfs(0, mid, 0, res1);
        dfs(mid, N, 0, res2);

        auto ans = -inf;
        debug {
            for (auto &i: res1)  io << i << ' ';
            io << endl;
            for (auto &i: res2)  io << i << ' ';
            io << endl;
        }
        for (auto &i: res2) {
            auto except = M - i;
            auto found = res1.lower_bound(except);
            if (found != res1.begin())  chkMax(ans, ((*--found) + i) % M);
            else  chkMax(ans, ((*res1.rbegin()) + i) % M);
        }
        
        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_4870535850952584::solve();
    return 0;
}