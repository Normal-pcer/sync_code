/**
 * @link https://atcoder.jp/contests/abc401/tasks/abc401_f
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define _lambda_1(expr) [&]() { return expr; }
#define _lambda_2(a, expr) [&](auto a) { return expr; }
#define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
#define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::uint16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;

namespace Solution_6152530545952873 {
    template <typename T, typename U>
    struct ItemSource {
        T value;
        U source;

        auto operator<=> (ItemSource const &) const = default;
    };
    auto solve() -> void {
        using Graph = std::vector<std::vector<i32>>;

        std::array<i32, 2> n;
        std::array<Graph, 2> t;

        for (i32 i: {0, 1}) {
            std::cin >> n[i];
            t[i] = Graph(n[i] + 1);
            for (auto _ = n[i] - 1; _ --> 0; ) {
                i32 x, y;
                std::cin >> x >> y;
                t[i][x].push_back(y);
                t[i][y].push_back(x);
            }
        }

        // 获取图上所有点到给定点的距离
        auto getDistance = [&](i32 s, i32 n, Graph const &graph, std::vector<i32> &dis) -> void {
            std::vector<char> vis(n + 1, false);

            std::deque<i32> q;
            q.push_back(s);
            dis[s] = 0;
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                if (vis[u]) continue;
                vis[u] = true;

                for (auto v: graph[u]) {
                    if (vis[v]) continue;
                    dis[v] = dis[u] + 1;
                    q.push_back(v);
                }
            }
        };

#if false  // 不会求树的直径导致的
        // 与直径相关的信息
        struct DiameterInfo {
            ItemSource<i32, std::pair<i32, i32>> total;  // 子树下的直径
            ItemSource<i32, i32> single;  // 以当前点为端点的最长链
        };
        auto getDiameterRecursion = [&](auto &&getDiameterRecursion, i32 u, i32 prev, Graph const &graph) -> DiameterInfo {
            std::vector<ItemSource<i32, i32>> lengthCollection;
            lengthCollection.reserve(graph[u].size());

            ItemSource<i32, std::pair<i32, i32>> totalAns{};
            for (auto v: graph[u]) {
                if (v == prev) continue;
                auto [total, single] = getDiameterRecursion(getDiameterRecursion, v, u, graph);
                chkMax(totalAns, total);
                lengthCollection.push_back({.value = single.value + 1, .source = single.source});
            }
            if (lengthCollection.size() == 0) {
                return {.total = {.value = 0, .source = {u, u}}, .single = {.value = 0, .source = u}};
            }
            if (lengthCollection.size() >= 2) {
                // 获取最大的两个数
                ranges::partial_sort(lengthCollection, lengthCollection.begin() + 1, ranges::greater{});
                chkMax(totalAns, {
                    .value = lengthCollection[0].value + lengthCollection[1].value,
                    .source = std::pair{lengthCollection[0].source, lengthCollection[1].source}
                });
            } else {
                chkMax(totalAns, {
                    .value = lengthCollection[0].value,
                    .source = std::pair{lengthCollection[0].source, u}
                });
            }
            
            return {.total = totalAns, .single = lengthCollection[0]};
        };
        auto getDiameter = [&](Graph const &graph) -> ItemSource<i32, std::pair<i32, i32>> {
            return getDiameterRecursion(getDiameterRecursion, 1, 0, graph).total;
        };
#else
        auto getDiameter = [&](Graph const &graph, i32 n) -> ItemSource<i32, std::pair<i32, i32>> {
            std::vector<i32> tmp(n + 1);
            getDistance(1, n, graph, tmp);

            auto first = static_cast<i32>(ranges::max_element(tmp.begin() + 1, tmp.end()) - tmp.begin());
            getDistance(first, n, graph, tmp);
            auto second = static_cast<i32>(ranges::max_element(tmp.begin() + 1, tmp.end()) - tmp.begin());
            auto dis = tmp[second];
            return {.value = dis, .source = {first, second}};
        };
#endif

        std::array<ItemSource<i32, std::pair<i32, i32>>, 2> dm{{getDiameter(t[0], n[0]), getDiameter(t[1], n[1])}};

        

        // dis_g[i] 表示图 g 上 i 点距离直径上端点的较大值
        std::array<std::vector<i32>, 2> dis;
        for (i32 p: {0, 1}) {
            dis[p].resize(n[p] + 1, 0);

            std::vector<i32> tmp(n[p] + 1);
            getDistance(dm[p].source.first, n[p], t[p], tmp);
            ranges::transform(dis[p], tmp, dis[p].begin(), ranges::max);

            getDistance(dm[p].source.second, n[p], t[p], tmp);
            ranges::transform(dis[p], tmp, dis[p].begin(), ranges::max);
        }

        // std::cerr << std::format("dm[{}] value = {}, source = ({}, {})", 0, dm[0].value, dm[0].source.first, dm[0].source.second) << std::endl;
        // std::cerr << std::format("dm[{}] value = {}, source = ({}, {})", 1, dm[1].value, dm[1].source.first, dm[1].source.second) << std::endl;

        auto originalMaxDiameter = std::max(dm[0], dm[1]).value;
        
        i64 ans = 0;

        // u 和 v 连接之后的直径，要么是 dis[0][u] + dis[1][v] + 1，要么是 originalMaxDiameter
#if false
        for (i32 u = 1; u <= n[0]; u++) {
            for (i32 v = 1; v <= n[1]; v++) {
                std::cerr << std::format("u = {}, v = {}, ans += {}", u, v, std::max(dis[0][u] + dis[1][v] + 1, originalMaxDiameter)) << std::endl;
                auto check = [&]() -> bool {
                    Graph newGraph(n[0] + n[1] + 2);
                    for (i32 i = 1; i <= n[0]; i++) {
                        newGraph[i] = t[0][i];
                    }
                    for (i32 i = 1; i <= n[1]; i++) {
                        newGraph[i + n[0]] = t[1][i];
                        for (auto &x: newGraph[i + n[0]]) x += n[0];
                    }
                    newGraph[u].push_back(v + n[0]);
                    newGraph[v + n[0]].push_back(u);

                    auto tmp = getDiameter(newGraph, n[0] + n[1]);
                    return std::max(dis[0][u] + dis[1][v] + 1, originalMaxDiameter) == tmp.value;
                };
                assert(check());
                ans += std::max(dis[0][u] + dis[1][v] + 1, originalMaxDiameter);
            }
        }
#else
        {
            std::vector<ItemSource<i32, i32>> sorted(n[1]);
            for (i32 v = 1; v <= n[1]; v++) {
                sorted[v - 1] = {.value = dis[1][v], .source = v};
            }
            ranges::sort(sorted);

            std::vector<i64> ps(n[1]);
            ranges::transform(sorted, ps.begin(), lam(x, dis[1][x.source]));
            std::partial_sum(ps.begin(), ps.end(), ps.begin());
            for (i32 u = 1; u <= n[0]; u++) {
                // dis[0][u] + dis[1][v] + 1
                // 找到第一个 >= originalMaxDiameter
                auto finding = originalMaxDiameter - dis[0][u] - 1;
                auto first = ranges::partition_point(sorted, lam(x, x.value < finding));

                auto cntLess = static_cast<i64>(first - sorted.begin());
                ans += cntLess * originalMaxDiameter;

                auto cntGreater = static_cast<i64>(sorted.end() - first);
                auto sumDis_1_v = first != sorted.begin()
                    ? ps.back() - ps[first - sorted.begin() - 1]
                    : ps.back();
                auto sumOther = cntGreater * (dis[0][u] + 1);
                ans += sumOther + sumDis_1_v;
                std::cerr << sumOther + sumDis_1_v << std::endl;
            }
        }
#endif
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6152530545952873::solve();
    return 0;
}
