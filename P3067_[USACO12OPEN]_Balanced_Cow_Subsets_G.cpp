/**
 * @link https://www.luogu.com.cn/problem/P3067
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1600440631359333 {

    int N;
    std::vector<int> w;

    std::unordered_map<int, std::unordered_set<int>> res;
    std::map<int, std::unordered_set<int>> res2;

    // 三种划分 - 集合 A，集合 B，不选择
    template <class T>
    void dfs(
        int p,      // 即将被处理的元素
        int diff,   // A - B
        T&& res,    // 输出结果
        int limit,
        int status = 0
    ) {
        never io.writeln(p, diff, limit, status);
        if (p >= limit) {  // 记录答案
            /*if (status != 0)*/  res[diff].insert(status);
            return;
        }
        dfs(p+1, diff, res, limit, status);  // 不选择
        dfs(p+1, diff + w[p], res, limit, status | 1<<p);  // +A
        dfs(p+1, diff - w[p], res, limit, status | 1<<p);  // +B
    }

    template <class T, class U>
    void dfs2(int p, int diff, T&& prev, U&& res, int limit, int status = 0) {
        never io.writeln("qwq", p, diff, limit, status);
        if (p >= limit) {
            /*if (status != 0)*/  res[diff].insert(status);
            return;
        }

        dfs2(p+1, diff, prev, res, limit, status);
        dfs2(p+1, diff + w[p], prev, res, limit, status | 1<<p);
        dfs2(p+1, diff - w[p], prev, res, limit, status | 1<<p);
    }

    void solve() {
        io >> N, w.resize(N);
        from(i, 0, N-1)  io >> w[i];

        auto mid = N >> 1;
        dfs(0, 0, res, mid+1, 0);
        never for (auto &[key, value]: res) { 
            debug io.write("L", __LINE__, key, ": ");
            for (auto &i: value)  io << i << ' ';
            io << endl;
        }
        dfs2(mid+1, 0, res, res2, N, 0);

        ll ans = 0;
        std::unordered_set<int> set;
        for (auto it = res2.lower_bound(0); it != res2.end(); it++) {
            auto& [diff, sts] = *it;
            auto required = -diff;
            ans += res[required].size() * sts.size();
            for (auto &i: res[required]) {
                for (auto &j: sts) {
                    set.insert(i|j);
                    debug io.writeln("Find ans", i|j);
                }
            }
        }

        io << set.size() - 1 << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1600440631359333::solve();
    return 0;
}
