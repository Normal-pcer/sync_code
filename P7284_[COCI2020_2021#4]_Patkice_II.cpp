/**
 * @link https://www.luogu.com.cn/problem/P7284
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
namespace lib {   
    template <typename T> auto range(const T to) {  return std::views::iota(T(), to);  }
    template <typename T> auto range(const T begin, const T end) {  return std::views::iota(begin, end);  }
    template <typename T, typename U> struct StepViewIterator {
        T _cur;  const U _step;
        StepViewIterator(T cur, const U step): _cur(cur), _step(step) {}
        auto operator++() { _cur+=_step; }  auto operator++(int) { _cur+=_step; }
        auto operator!=(const StepViewIterator& other) { return _step>=0? _cur<other._cur: _cur>other._cur; }
        auto operator*() { return _cur; }
    };
    template <typename T, typename U> struct StepView: std::ranges::view_interface<StepView<T, U>> {
        const T _begin, _end;  const U _step;
        StepView(const T begin, const T end, const U step): _begin(begin), _end(end), _step(step) {}
        auto begin() const { return StepViewIterator<T, U>(_begin, _step); }
        auto end() const { return StepViewIterator<T, U>(_end, _step); }
    };
    template <typename T, typename U> auto range(T begin, T end, U step) { return StepView<T, U>(begin, end, step); }
}
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
