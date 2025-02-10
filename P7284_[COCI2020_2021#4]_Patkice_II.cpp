/**
 * @link https://www.luogu.com.cn/problem/P7284
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1356823277714342 {
    struct MaxItem {
        int val = -1, src = -1;

        auto operator<=> (const MaxItem &) const = default;
    };
    class Graph {
        struct Forward {
            int to, val;
        };
        int N;
        std::vector<std::vector<Forward>> graph;
    public:
        Graph(int N): N(N), graph(N) {}
        auto checkPoint(int p) const -> bool {
            return 0 <= p and p < N;
        }
        auto add(int x, int y, int val) -> void {
            // 忽略不合法的点
            if (not checkPoint(x))  return;
            if (not checkPoint(y))  return;

            debug  std::cout << std::format("add {} {} {}", x, y, val) << std::endl;
            graph.at(x).push_back({y, val});
        }
        auto query(int outset, int target) const -> auto {
            std::vector<MaxItem> F(graph.size());
            struct QueueValueType {
                int cur;
                int prev = -1;
                int val = 0;
                auto operator<=> (const QueueValueType &other) const -> std::weak_ordering {
                    return val <=> other.val;
                }
            };
            std::priority_queue<QueueValueType, std::vector<QueueValueType>, std::greater<>> q;
            q.push({outset});
            while (not q.empty()) {
                auto [cur, prev, val] = q.top();  q.pop();
                debug  std::cout << cur << " " << prev << " " << val << std::endl;
                if (F[cur].val != -1)  continue;
                F[cur] = {val, prev};
                if (cur == target)  return std::pair{val, F};

                for (auto [x, del]: graph.at(cur)) {
                    if (F[x].val == -1)  q.push({x, cur, val + del});
                }
            }
            assert(false), __builtin_unreachable();
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        int M;  std::cin >> M;

        std::vector<std::string> map(N);
        for (auto &line: map) {
            line.resize(M);
            std::cin >> line;
            assert(static_cast<int>(line.size()) == M);
        }

        Graph g(N * M);
        auto pointIndex = [&](int x, int y) -> int {
            if (x < 0 or x >= N)  return -1;
            if (y < 0 or y >= M)  return -1;
            return x * M + y; 
        };
        int x0 = -1, y0 = -1, x1 = -1, y1 = -1;
        for (auto i: range(N)) {
            for (auto j: range(M)) {
#define DIR(dx, dy)     do { for (auto x: {0x21, 0x01, 0x12, 0x10}) { \
                            auto cur_dx = (x >> 4) - 1, cur_dy = (x & 15) - 1; \
                            if (cur_dx == dx and cur_dy == dy)  g.add(pointIndex(i, j), pointIndex(i + cur_dx, j + cur_dy), 0); \
                            else  g.add(pointIndex(i, j), pointIndex(i + cur_dx, j + cur_dy), 1); \
                        } } while(false)

                if (map[i][j] == 'o')  {
                    x0 = i, y0 = j;
                    for (auto x: {0x21, 0x01, 0x12, 0x10}) {
                        auto cur_dx = (x >> 4) - 1, cur_dy = (x & 15) - 1;
                        g.add(pointIndex(i, j), pointIndex(i + cur_dx, j + cur_dy), 0);
                    }
                }
                else if (map[i][j] == 'x')  x1 = i, y1 = j;
                else if (map[i][j] == '<')  DIR(0, -1);
                else if (map[i][j] == '>')  DIR(0, +1);
                else if (map[i][j] == 'v')  DIR(+1, 0);
                else if (map[i][j] == '^')  DIR(-1, 0);
                else if (map[i][j] == '.') {
                    for (auto x: {0x21, 0x01, 0x12, 0x10}) {
                        auto cur_dx = (x >> 4) - 1, cur_dy = (x & 15) - 1;
                        g.add(pointIndex(i, j), pointIndex(i + cur_dx, j + cur_dy), 1);
                    }
                }
                else  assert(false), __builtin_unreachable();
            }
        }
        assert(x0 != -1 and y0 != -1 and x1 != -1 and y1 != -1);

        auto [ans, F] = g.query(pointIndex(x0, y0), pointIndex(x1, y1));
        std::cout << ans << std::endl;

        auto cur = pointIndex(x1, y1);
        while (cur != -1) {
            auto prev = F[cur].src;
            if (prev == -1)  break;
            auto prev_x = prev / M, prev_y = prev % M;
            auto &ch = map[prev_x][prev_y];
            if (ch != 'x' and ch != 'o') {
                if (cur == prev + 1)  ch = '>';
                else if (cur == prev - 1)  ch = '<';
                else if (cur == prev + M)  ch = 'v';
                else if (cur == prev - M)  ch = '^';
                else  assert(false), __builtin_unreachable();
            }
            cur = prev;
        }

        for (auto const &line: map) {
            std::cout << line << endl;;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1356823277714342::solve();
    return 0;
}
