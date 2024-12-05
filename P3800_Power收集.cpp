/**
 * @link https://www.luogu.com.cn/problem/P3800
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3541765236282983 {
    struct Point {
        int x, y, val;
    };
    void solve() {
        int M, N, K, T;  io >> N >> M >> K >> T;
        std::vector<Point> luo(K);
        std::vector F(N+1, std::vector<ll>(M+1));

        for (auto &[x, y, val]: luo)  io >> x >> y >> val, F.at(x).at(y) = val;

        for (auto i: range(2, N+1)) {
            std::deque<int> q;  // 递减
            for (auto j: range(1, T+1)) {
                while (not q.empty() and F.at(i-1).at(j) > F.at(i-1).at(q.back()))  q.pop_back();
                q.push_back(j);
            }
            for (auto j: range(1, M+1)) {
                if (j + T <= M) {
                    while (not q.empty() and F.at(i-1).at(j+T) >= F.at(i-1).at(q.back()))  q.pop_back();
                    q.push_back(j + T);
                }
                while (not q.empty() and q.front() + T < j)  q.pop_front();
                assert(not q.empty());
                F.at(i).at(j) += F.at(i-1).at(q.front());
            }
        }

        io << *std::max_element(F.at(N).begin(), F.at(N).end()) << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_3541765236282983::solve();
    return 0;
}
