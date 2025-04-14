/**
 * @link https://www.luogu.com.cn/problem/P3293
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
从高位向低位依次判断这一位的答案是否可能为 1。

假设已经确定当前最优的答案为 0b111______
确定答案的下一位？

如果 [0b111100000, 0b111111111] 内存在一个 k，使得 (k xor bi) - xi 存在合法的菜，这一位就可以是 1。
k xor bi 的高四位是确定的，低位随意，所以 k xor bi 的值域也是连续的这么多个。
减去 xi 之后也是一个连续区间。

于是只要在线段树上查询这个连续区间就好了。由于限制了 l 和 r，需要使用可持久化线段树来维护。
*/
namespace Solution_4492015718158797 {
    class PersistentSegTree {
        struct Node {
            i32 sum = 0;
            i32 left = 0, right = 0;
        };
        std::deque<Node> nodes;

        i32 root = 0;
        i32 rootBegin = 0, rootEnd = 0;

        std::vector<i32> rootOfVersion;

        auto pushUp(i32 p) -> void {
            nodes[p].sum = nodes[nodes[p].left].sum + nodes[nodes[p].right].sum;
        }

        auto addAt(i32 pos, i32 val, i32 &p, i32 nodeBegin, i32 nodeEnd) -> void {
            nodes.emplace_back(nodes[p]);
            p = static_cast<i32>(nodes.size()) - 1;
            if (nodeEnd - nodeBegin == 1) {
                assert(nodeBegin == pos);
                nodes[p].sum += val;
                return;
            }
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            if (pos < nodeMid) addAt(pos, val, nodes[p].left, nodeBegin, nodeMid);
            else addAt(pos, val, nodes[p].right, nodeMid, nodeEnd);
            pushUp(p);
        }

        // 统计 (p1, p0]
        auto sumRangeBetweenVersion(i32 begin, i32 end, i32 p1, i32 p0, i32 nodeBegin, i32 nodeEnd) -> i32 {
            if (nodeBegin >= begin and nodeEnd <= end) {
                return nodes[p0].sum - nodes[p1].sum;
            }
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            i32 ans = 0;
            if (begin < nodeMid) ans += sumRangeBetweenVersion(begin, end, nodes[p1].left, nodes[p0].left, nodeBegin, nodeMid);
            if (end > nodeMid) ans += sumRangeBetweenVersion(begin, end, nodes[p1].right, nodes[p0].right, nodeMid, nodeEnd);
            return ans;
        }
    public:
        PersistentSegTree(i32 rootBegin, i32 rootEnd, i32 maxVersion)
                : rootBegin(rootBegin), rootEnd(rootEnd), rootOfVersion(maxVersion + 1) {
            nodes.push_back({});
            rootOfVersion.push_back(0);
        }

        auto addAt(i32 baseVersion, i32 newVersion, i32 pos, i32 val) -> void {
            assert(baseVersion < static_cast<i64>(rootOfVersion.size()));
            rootOfVersion[newVersion] = rootOfVersion[baseVersion];
            addAt(pos, val, rootOfVersion[newVersion], rootBegin, rootEnd);
        }

        auto sumRangeBetweenVersion(i32 begin, i32 end, i32 oldVersion, i32 version) -> i32 {
            begin = std::clamp(begin, rootBegin, rootEnd);
            end = std::clamp(end, rootBegin, rootEnd);
            if (begin == end) return 0;
            return sumRangeBetweenVersion(
                begin, end, rootOfVersion[oldVersion], rootOfVersion[version], rootBegin, rootEnd);
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i32> a(n + 1);
        for (auto &x: a | views::drop(1)) std::cin >> x;

        // 权值线段树
        auto sgtBegin = ranges::min(a);
        auto sgtEnd = ranges::max(a) + 1;
        PersistentSegTree sgt(sgtBegin, sgtEnd, n);
        for (i32 i = 1; i <= n; i++) {
            sgt.addAt(i - 1, i, a[i], 1);
        }

        for (auto _ = m; _ --> 0; ) {
            i32 b, x, l, r;
            std::cin >> b >> x >> l >> r;
            
            // 从高到低，依次枚举二进制位
            i32 constexpr maxBit = 19;
            i32 ans = 0;
            for (i32 p = maxBit; p --> 0; ) {
                auto mask = ans | (1 << p);
                mask ^= b & (-1u << p);

                auto min = mask - x;
                auto has = sgt.sumRangeBetweenVersion(min, min + (1 << p), l - 1, r) != 0;
                if (has) {
                    ans |= 1 << p;
                }
            }
            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4492015718158797::solve();
    return 0;
}
