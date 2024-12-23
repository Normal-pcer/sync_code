/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3275
 */

#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1115862076892276 {
    const int _V = 5002; 
    std::array<std::array<int, _V>, _V> diff;
#define at operator[]
    void solve() {
        int N;  io >> N;
        int x1, x2, y1, y2;
        // auto max_x = -inf, max_y = -inf;
        for (auto i = 0; i < N; i++) {
            io >> x1 >> y1 >> x2 >> y2;
            x1++, y1++, x2++, y2++;
            diff[x1][y1]++, diff[x2][y2]++;
            diff[x1][y2]--, diff[x2][y1]--;
            // chkMax(max_x, x2), chkMax(max_y, y2);
        }
        auto cnt = 0;
        for (auto i = 1; i < 5000; i++) {
            for (auto j = 1; j < 5000; j++) {
                diff[i][j] += diff[i-1][j] + diff[i][j-1] - diff[i-1][j-1];
                if (diff[i][j] != 0)  cnt++;
            }
        }
        io << cnt << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1115862076892276::solve();
    return 0;
}
