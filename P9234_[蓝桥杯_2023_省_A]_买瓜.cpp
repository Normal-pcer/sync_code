/**
 * 
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using namespace lib;

namespace Solution_1806430963834722 {

    const int _N = 35;
    int N, M;
    std::vector<int> w;
    // std::vector<ll> ps;

    std::map<int, int> res_l;  // 达到目标重量最少劈瓜次数

    int limitPos;
    auto best = inf;

    void _dfs(int p, int sum, int broken) {
        // if (broken >= best)  return;
        if (sum > M)  return;
        // if (p<N and (ll)sum + ps[p] < (ll)M)  return;
        if (p >= limitPos) {
            if (res_l.find(sum) == res_l.end())   res_l.insert({sum, broken});
            else {
                auto& target = res_l.at(sum);
                chkMin(target, broken);
            }
            // chkMin(best, broken);
            return;
        }
        _dfs(p+1, sum, broken);
        _dfs(p+1, sum + (w[p]>>1), broken+1);
        _dfs(p+1, sum + w[p], broken);
    }

    void _dfs2(int p, int sum, int broken) {
        // if (broken >= best)  return;
        // if (std::clock() > limit)  return;
        if (sum > M)  return;
        // if (p<N and (ll)sum + ps[p] < (ll)M)  return;
        if (broken > best)  return;
        if (p >= limitPos) {
            auto required = M - sum;
            if (res_l.find(required) != res_l.end()) {
                auto append = res_l.at(required);
                auto res = broken + append;
                chkMin(best, res);
            }
            // chkMin(best, broken);
            return;
        }
        _dfs2(p+1, sum, broken);
        _dfs2(p+1, sum + (w[p]>>1), broken+1);
        _dfs2(p+1, sum + w[p], broken);
    }

    void solve() {
        io >> N >> M, M *= 2;
        w.resize(N);
        from(i, 0, N-1)  io >> w[i], w[i] *= 2;
        std::sort(w.begin(), w.end());
        // ps.resize(N);
        // std::partial_sum(w.rbegin(), w.rend(), ps.rbegin());
        // debug for (auto &i: ps)  io << i << endl;

        auto mid = N * 3 >> 3;  // 左侧分 3/8
        limitPos = mid+1;
        _dfs(0, 0, 0);
        limitPos = N;
        _dfs2(mid+1, 0, 0);

        if (best >= inf)  io << -1 << endl;
        else  io << best << endl;
    }
}

int main() {
    initDebug;
    Solution_1806430963834722::solve();
    return 0;
}