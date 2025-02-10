/**
 * @link https://www.luogu.com.cn/problem/P8405
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

namespace Solution_1499991513624592 {
    struct Graph {
        int N;
        std::vector<std::pair<int, int>> edges;
        std::vector<std::vector<int>> graph;

    public:
        Graph(int N): N(N), graph(N+1) {}
        auto add(int x, int y) -> void {
            graph.at(x).push_back(y);
            edges.push_back({x, y});
        }
        auto topo() const -> std::optional<std::vector<int>> {
            std::vector<int> in(N+1);  // 入度
            for (auto [_, y]: edges)  in.at(y)++;

            std::deque<int> q;
            for (auto i: range(1, N+1)) {
                if (in[i] == 0)  q.push_back(i);
            }

            std::vector<int> order;
            auto cnt = 0;
            while (not q.empty()) {
                auto x = q.front();  q.pop_front();
                cnt++;
                
                for (auto y: graph.at(x)) {
                    in[y]--;
                    if (in[y] == 0)  q.push_back(y), order.push_back(y);
                }
            }

            if (cnt == N)  return order;
            else  return std::nullopt;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        int M;  std::cin >> M;
        Graph graph{N};
        for (auto _: range(M)) {
            int x, y;  std::cin >> y >> x;
            graph.add(x, y);
        }

        auto order_or_null = graph.topo();
        if (order_or_null == std::nullopt) {
            std::cout << -1 << std::endl;
            return;
        }
        auto &order = *order_or_null;
        std::cout << order.size() << endl;
        for (auto x: order) {
            std::cout << x << " " << 1 << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1499991513624592::solve();
    return 0;
}
