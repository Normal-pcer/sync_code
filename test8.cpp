/**
 * 
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;


namespace Solution_1469017748636242 {
    void solve() {
        std::vector<std::vector<int>> matrix(4, std::vector<int>(4));

        from(i, 0, 4) {
            from(j, 0, 4) {
                auto x = io.get();
                matrix.at(i).at(j) = x - 1;
            }
        }

        std::vector<std::pair<int, int>> limits;

        // Corner
        from(i, 0, 3) {
            from(j, 0, 3) {
                from(dx, 0, 2) {
                    from(dy, 0, 2) {
                        auto x = i + dx, y = j + dy;
                        auto cur = j + 3*i;
                        if (matrix.at(x).at(y) != cur)  limits.push_back({cur, matrix[x][y]});
                    }
                }
            }
        }

        std::vector<std::vector<int>> graph(9);
        std::vector<int> in(9);
        std::vector<int> order;
        for (auto [down, up]: limits) {
            debug io << "limits" << down << ',' << up << endl;
            graph.at(down).push_back(up);
            in.at(up)++;
        }

        std::deque<int> q;
        from(i, 0, 3) {
            if (in.at(i) == 0)  q.push_back(i);
        }

        while (not q.empty()) {
            auto x = q.front();  q.pop_front();
            order.push_back(x);

            for (auto dest: graph.at(x)) {
                if (--in.at(dest) == 0)  q.push_back(dest);
            }
        }

        debug {
            for (auto i: order)  io << i << ' ';
            io << endl;
        }

        if (order.size() == 9ULL) {
            io << "WINDOWS ARE OK" << endl;
        } else {
            io << "WINDOWS ARE BROKEN" << endl;
        }
    }
}


int main() {
    initDebug;
    int T;  io >> T;
    while (T --> 0)  Solution_1469017748636242::solve();
    return 0;
}