/**
 * @link https://www.luogu.com.cn/problem/P3512
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1372311588664402 {
    void solve() {
        int K, N;  io >> K >> N;
        std::vector<int> a(N);
        for (auto &i: a)  io >> i;

        std::deque<int> inc;  // 递增队列
        std::deque<int> dec;  // 递减队列

        int ans = 0;

        for (auto pt2 = 0; pt2 != (int)a.size(); pt2++) {
            while (not inc.empty() and not dec.empty() and - a.at(dec.front()) + a.at(inc.front()) > K) {
                if (inc.front() < dec.front())  inc.pop_front();
                else  dec.pop_front();
            }
            
            while (not inc.empty() and a.at(inc.back()) <= a.at(pt2))  inc.pop_back();
            while (not dec.empty() and a.at(dec.back()) >= a.at(pt2))  dec.pop_back();
            inc.push_back(pt2), dec.push_back(pt2);

            if (not inc.empty() and not dec.empty()) {
                auto max = a.at(inc.front()), min = a.at(dec.back());
                // io << '!' << max << ' ' << min << endl;
                if (max - min <= K) {
                    chkMax(ans, std::max(inc.front(), dec.front()) - std::min(inc.front(), dec.front()));
                }
            }

            // for (auto i: inc)  io << std::format("[{}]{}", i, a.at(i)) << ' ';
            // io << endl;
            // for (auto i: dec)  io << std::format("[{}]{}", i, a.at(i)) << ' ';
            // io << endl;
        }

        io << ans << endl;
    }
}

int main() {
    initDebug;
    Solution_1372311588664402::solve();
    return 0;
}
