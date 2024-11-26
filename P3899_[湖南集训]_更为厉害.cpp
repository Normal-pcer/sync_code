/**
 * @link https://www.luogu.com.cn/problem/P3899
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)
#define main() main(int argc, char const *argv[])
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
#define debug if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{}
#if __cplusplus > 201703LL
namespace ranges { using namespace std::ranges; }
namespace views { using namespace std::ranges::views; }
#endif

#include "./libs/range.hpp"

#include "./libs/io.hpp"
#include "./libs/__vector.hpp"

using namespace lib;

namespace Solution_7725643738033283 {

    unstd::vector<unstd::vector<int>> graph;

    unstd::vector<int> order;  // dfs 序
    unstd::vector<int> size;  // 子树大小
    unstd::vector<int> depth {-1};  // 根节点深度为 0
    unstd::vector<std::pair<int, int>> subTreeRanges;  // 子树范围，左闭右开
    auto dfs_ordering(int p, int prev) -> void {
        order.push_back(p);
        size.at(p) = 1, depth.at(p) = depth.at(prev) + 1;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_ordering(dest, p);
            size.at(p) += size.at(dest);
        }
    }

    unstd::vector<ll> c(524288);  // 在某个深度上有多少个点
    inline int lowbit(int x) { return x & -x; }

    auto query(int x) {
        debug std::cout << std::format("query({})", x) << std::endl;
        x++;
        auto res = 0LL;
        while (x) {
            res += c.at(x);
            x -= lowbit(x);
        }
        return res;
    }

    void update(int x, int val) {
        debug std::cout << std::format("update({}, {})", x, val) << std::endl;
        x++;
        while (x < (int)c.size()) {
            c.at(x) += val;
            x += lowbit(x);
        }
    }

    struct QuerySeg {
        int begin, end;
        int p, k;

        QuerySeg(int begin=0, int end=0, int p=0, int k=0): begin(begin), end(end), p(p), k(k) {}
        QuerySeg(std::pair<int, int> seg, int p, int k):
            begin(seg.first), end(seg.second), p(p), k(k) {} 
    };

    struct KeyPoint {
        int point, p, k;

        auto operator<=> (const KeyPoint &) const = default;
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  io >> N >> Q;
        graph.resize(N+1);
        for (auto _: range(N-1)) {
            int x, y;  io >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        size.resize(N+1), depth.resize(N+1), dfs_ordering(1, 0);
        always {
            unstd::vector<int> pos(N+1);
            for (auto i: range(order.size())) {
                pos.at(order.at(i)) = i;
            }
            subTreeRanges.resize(N+1);
            for (auto i: range(1, N+1)) {
                auto begin = pos.at(i);
                auto end = begin + size.at(i);
                subTreeRanges.at(i) = {begin, end};

                debug std::cout << std::format("Point {}: [{}, {})", i, begin, end) << std::endl;
            }
        }

        unstd::vector<std::pair<int, int>> queries(Q);
        for (auto &[p, k]: queries)  io >> p >> k;

        // 需要记录一些 dfs 序上的“关键点”及其附加的 k
        unstd::vector<QuerySeg> segs(Q);  // 待查询的区间
        ranges::transform(queries, segs.begin(), [&](auto x) {
            auto [p, k] = x;
            return QuerySeg(subTreeRanges.at(p), p, k);
        });

        unstd::vector<KeyPoint> keyPoints;  // 需要考虑的点（在 dfs 序上）
        ranges::transform(segs, std::back_inserter(keyPoints), lam(seg, KeyPoint({seg.begin, seg.p, seg.k})));
        ranges::transform(segs, std::back_inserter(keyPoints), lam(seg, KeyPoint({seg.end, seg.p, seg.k})));
        ranges::sort(keyPoints, lam(x, y, x.point<y.point));

        debug for (auto [x, p, k]: keyPoints)  std::cout << std::format("Key point: x = {}, p = {}, k = {}", x, p, k) << std::endl;

        unstd::vector<ll> F;
        auto point_to_be_insert = 0;  // 下一次放置的点
        for (auto [x, p, k]: keyPoints) {
            while (point_to_be_insert < x) {
                auto point = order.at(point_to_be_insert++);  // 被选中的点
                update(depth.at(point), size.at(point) - 1); // 权值为子树大小减一
            }
            F.push_back(query(depth.at(p)+k) - query(depth.at(p)));
        }

        std::map<KeyPoint, ll> F_map;
        for (auto i: range(keyPoints.size())) {
            F_map.insert({keyPoints.at(i), F.at(i)});
            debug std::cout << std::format("{{{}, {}, (depth ({}, {}])}} -> {}", keyPoints.at(i).point, 
                    keyPoints.at(i).p, depth.at(keyPoints.at(i).p), 
                    depth.at(keyPoints.at(i).p)+keyPoints.at(i).k, F.at(i)
                ) << std::endl;
        }

        for (auto [p, k]: queries) {
            auto ans = 0LL;

            // B 是 A 的父亲时
            // 对于 B，可以从 A 一直向上取，直到达到 k 个或者到达根节点
            // 对于 C，A 的所有子树（除了它本身）都可行
            ans += (ll)std::min(depth.at(p), k) * (size.at(p) - 1);

            // A 是 B 的父亲时
            // B 可以选在一定深度限制区域内
            auto seg = subTreeRanges.at(p);
            auto choice = F_map.at({seg.second, p, k}) - F_map.at({seg.first, p, k});
            // C 只要是子树（也不含根）即可
            ans += choice;

            std::cout << ans << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_7725643738033283::solve();
    return 0;
}