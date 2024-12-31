/**
 * @link https://www.luogu.com.cn/problem/P1941
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/debug_log.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_8082753816483782 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        struct Tube {
            int pos;
            int down, up;
        };
        struct Behaviour {
            int click, miss;
        };
        std::vector<Behaviour> behaviours(N);
        for (auto &[click, miss]: behaviours)  std::cin >> click >> miss;
        std::vector<Tube> tubes(K);
        for (auto &[pos, down, up]: tubes)  std::cin >> pos >> down >> up;
        ranges::sort(tubes, ranges::less{}, lam(x, x.pos));

        auto tube_it = tubes.begin();
        std::vector F(N+1, std::vector(M+1, inf));
        ranges::fill(F[0], 0);
        for (auto i: range(1, N+1)) {
            for (auto j: range(1, M+1)) {
                chkMin(F[i][std::min(M, j+behaviours[i-1].click)], F[i-1][j] + 1);
                chkMin(F[i][std::min(M, j+behaviours[i-1].click)], F[i][j] + 1);
                if (j-behaviours[i-1].miss > 0)  chkMin(F[i][j-behaviours[i-1].miss], F[i-1][j]);
                if (j-behaviours[i-1].miss > 0)  chkMin(F[i][j-behaviours[i-1].miss], F[i][j]);
            }
            if (tube_it != tubes.end() and i == tube_it->pos) {
                for (auto j: range(0, tube_it->down + 1))  F[i][j] = inf;
                for (auto j: range(tube_it->up, M+1))  F[i][j] = inf;
                tube_it++;
            }
        }

        debug {
            for (auto i: range(N+1)) {
                for (auto j: range(1, M+1)) {
                    debug {
                        printValues(i, j, F[i][j]);
                    }
                }
            }
        }

        auto min = ranges::min(F[N]);
        if (min != inf) {
            std::cout << 1 << endl << min << endl;
            return;
        }
        for (auto i: range(tubes.size()) | views::reverse) {
            auto min = ranges::min(F[tubes[i].pos]);
            if (min != inf) {
                std::cout << 0 << endl << i + 1 << endl;
                return;
            }
        }
        std::cout << 0 << endl << 0 << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8082753816483782::solve();
    return 0;
}
