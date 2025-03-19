/**
 * @link https://www.luogu.com.cn/problem/P10281
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 考虑和一个线段的交集不超过 K 需要满足什么条件。
 * x1         y1
 * |__________|
 *      |_______|
 *      x2      y2
 * 
 * 要么是类似这种情况，seg2 在偏右的位置。
 * 这个时候需要满足：
 * y1 >= x2 + K, y2 >= x2 + K (a)
 *     x1      y1
 *     |_______|
 * |______|
 * x2     y2
 * 要么是这种情况，满足：
 * y2 >= x1 + K, y2 >= x2 + K (b)
 * x1        y1
 * |_________|
 *    |____|
 *    x2   y2
 * 两项同时满足，当且仅当：
 * x2 >= x1, y2 <= y1, y2 >= x2 + K (c)
 * 此外，还有：
 *    x1      y1
 * x2 |_______|   y2
 * |______________|
 * x2 <= x1, y2 >= y1, y1 >= x1 + K (d)
 * 
 * (c) 很像三维偏序形式，(a)(b)(d) 是二维偏序。
 * 看起来有点麻烦，但还是先考虑一下实现。
 * 
 * 将上述计数抽象为若干次“修改”和“查询”操作，修改即放置一个指定 {x, y} 值的元素，查询即给定 {x1, y1}，数有多少个放置过的元素满足 x2 <= x1, y2 <= y1。
 * 
 * 弄错了。(a) 还需要加一条 x2 >= x1。(b) 同理。
 * 先别急。通过和暴力检验，三维偏序部分大概率没什么问题。
 * 再来仔细考虑一下线段相交的情况。先把线段 (x1, y1) 放在这里。
 *     |_______|
 *  $  x1  $   y1   $
 * 每个美元符号对应的位置都可以放置另一条线段的端点。所有所有的情况无非就 6 种。
 * 去掉不可能有交集的两种，一共就只有上面画过的四种情况了。
 * 
 * |____|
 *   |____|
 * x1 <= x2, x2 <= y1, y1 <= y2, y1 - x2 >= K
 * 即 -x2 <= -x1, x2 <= y1 - K, -y2 <= -y1
 * 
 *   |____|
 * |___|
 * x2 <= x1, x1 <= y2, y2 <= y1, y2 - x1 >= K
 * 即 x2 <= x1, -y2 <= x1 - K, y2 <= y1
 * 
 * |_______|
 *    |___|
 * x1 <= x2, x2 <= y2, y2 <= y1, y2 - x2 >= K
 * 即 -x2 <= -x1, -y2 <= -K - x2, y2 <= y1
 * 
 *    |__|
 * |_______|
 * x2 <= x1, x1 <= y1, y1 <= y2, y1 - x1 >= K
 * 即 x2 <= x1, -y2 <= -y1, y1 - x1 >= K （这个其实只是二维偏序）
 * 
 * 然而它无法正确地处理边界情况。即两个线段有端点重合。
 * |______|
 * |__________|
 * |___|
 * 
 * 考虑去重。有一个端点重合的会被计算恰好 2 次，两个端点重合的会被计算恰好 4 次。
 * 在每个端点上分别做一遍一维偏序即可。（什么东西）
 */
namespace Solution_2107735140546012 {
    struct Segment {
        i32 begin = 0, end = 0;
        i32 limit = 0;
    };
    void solve_force() {
        i32 N;  std::cin >> N;

        std::vector<Segment> segs(N);
        for (auto &[x, y, z]: segs)  std::cin >> x >> y >> z;

        auto seg_intersection = [&](Segment seg1, Segment seg2) -> i32 {
            i32 x1 = seg1.begin, y1 = seg1.end;
            i32 x2 = seg2.begin, y2 = seg2.end;
            if (x1 > x2)  std::swap(x1, x2), std::swap(y1, y2);
            // x1 <= x2 <= y2
            if (y1 <= x2)  return 0;
            else if (y1 <= y2)  return y1 - x2;
            else  return y2 - x2;
        };

        for (auto const &seg1: segs) {
            auto cnt = ranges::count_if(segs, lam(const &seg2, seg_intersection(seg1, seg2) >= seg1.limit and &seg2 != &seg1));
            std::cout << cnt << endl;
        }
    }
    class BIT {
        std::vector<std::pair<i32, i64>> ops;
        std::vector<i64> c;

        auto constexpr static lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT() {}
        BIT(i32 N): c(N + 1) {}

        auto addAt(i32 x, i64 val) -> void {
            ops.emplace_back(x, val);
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(i32 x) -> i64 {
            x++;  i64 res = 0;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
        auto clear() -> void {
            decltype(ops) ops_copy;
            ops.swap(ops_copy);

            for (auto [x, val]: ops_copy) {
                addAt(x, -val);
            }
            ops.clear();
        }
    };
    // 二维偏序
    class PartialOrderCounter2D {
    public:
        struct Element {
            enum Type { Update, Query } type = Update;
            i32 x = 0, y = 0;
            i32 index = 0;
        };

        std::vector<Element> ele;
        PartialOrderCounter2D() {}
        PartialOrderCounter2D(std::vector<Element> ele): ele(std::move(ele)) {}

        auto count() -> std::vector<i64> {
            auto max_index = ranges::max( ele | views::transform(lam(x, x.index)) );
            auto max_y = ranges::max( ele | views::transform(lam(x, x.y)) );
            std::vector<i64> ans(max_index + 1);

            ranges::partition(ele, lam(x, x.type == Element::Update));  // 优先更新
            ranges::stable_sort(ele, std::less{}, lam(x, x.x));  // 按照 x 排序
            BIT bit{max_y + 1};

            for (auto e: ele) {
                if (e.type == Element::Update) {
                    bit.addAt(e.y, 1);
                    assert(e.y >= 0);
                } else {
                    if (e.y >= 0) {
                        auto cnt = bit.sumPrefix(e.y);
                        ans[e.index] += cnt;
                    }
                }
            }
            return ans;
        }
    private:
    };
    // 三维偏序
    class PartialOrderCounter3D {
    public:
        struct Element {
            enum Type { Update, Query } type = Update;
            i32 x = 0, y = 0, z = 0;
            i32 index = 0;
        };

        std::vector<Element> ele;
        PartialOrderCounter3D() {}
        PartialOrderCounter3D(std::vector<Element> ele): ele(std::move(ele)) {}

        auto count() -> std::vector<i64> {
            auto max_z = ranges::max( ele | views::transform(lam(x, x.z)) );
            auto max_index = ranges::max( ele | views::transform(lam(x, x.index)) );

            BIT bit{max_z + 1};
            std::vector<i64> ans(max_index + 1);

            ranges::partition(ele, lam(e, e.type == Element::Update));
            ranges::stable_sort(ele, std::less{}, lam(e, e.x));
            cdq(ele.begin(), ele.end(), bit, ans);
            return ans;
        }
    private:
        using ElementIter = std::vector<Element>::iterator;
        // 按照 y 进行归并排序的同时统计答案
        auto cdq(ElementIter begin, ElementIter end, BIT &bit, std::vector<i64> &ans) -> void {
            if (end - begin == 1)  return;
            auto mid = begin + std::midpoint<uz>(0, end - begin);
            cdq(begin, mid, bit, ans), bit.clear();
            cdq(mid, end, bit, ans), bit.clear();

            auto it1 = begin, it2 = mid;
            while (it1 != mid or it2 != end) {
                if (it2 == end or (it1 != mid and it1->y <= it2->y)) {
                    if (it1->type == Element::Update) {
                        assert(it1->z >= 0);
                        bit.addAt(it1->z, 1);
                    }
                    it1++;
                } else {
                    if (it2->type == Element::Query and it2->z >= 0) {
                        auto cnt = bit.sumPrefix(it2->z);
                        ans[it2->index] += cnt;
                    }
                    it2++;
                }
            }
            ranges::inplace_merge(begin, mid, end, std::less{}, lam(e, e.y));
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<Segment> segs(N);
        std::vector<i32> values;
        for (auto &[x, y, z]: segs) {
            std::cin >> x >> y >> z;
            values.insert(values.end(), {x, y, -x, -y, x - y});
        }

        auto get_index = [&](i32 x) -> i32 {
            auto it = ranges::upper_bound(values, x);
            assert(it != values.begin());
            return std::distance(values.begin(), std::prev(it));
        };

        {
            ranges::sort(values);
            auto tmp = ranges::unique(values);
            values.erase(tmp.begin(), tmp.end());
        }

        using Element2d = PartialOrderCounter2D::Element;
        using Element3d = PartialOrderCounter3D::Element;

        std::vector<i64> ans(N);

        // for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl;
        // x1 <= x2, x2 <= y1, y1 <= y2, y1 - x2 >= K
        // 即 -x2 <= -x1 (x), x2 <= y1 - K (z), -y2 <= -y1 (y)
        {
            std::vector<Element3d> ele;
            for (i32 i = 0; i < N; i++) {
                auto const &seg = segs[i];
                ele.push_back({.type = Element3d::Update, .x = -seg.begin, .y = -seg.end, .z = seg.begin});
                ele.push_back({.type = Element3d::Query, .x = -seg.begin, .y = -seg.end, .z = seg.end - seg.limit, .index = i});
            }
            for (auto &e: ele)  e.x = get_index(e.x), e.y = get_index(e.y), e.z = get_index(e.z);
            auto cur = PartialOrderCounter3D(std::move(ele)).count();
            ranges::transform(ans, cur, ans.begin(), std::plus{});
            // for (auto x: cur)  std::cout << x << " ";
            // std::cout << std::endl;
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        // x2 <= x1, x1 <= y2, y2 <= y1, y2 - x1 >= K
        // 即 x2 <= x1 (x), -y2 <= -x1 - K (y), y2 <= y1 (z)
        {
            std::vector<Element3d> ele;
            for (i32 i = 0; i < N; i++) {
                auto const &seg = segs[i];
                ele.push_back({.type = Element3d::Update, .x = seg.begin, .y = -seg.end, .z = seg.end});
                ele.push_back({.type = Element3d::Query, .x = seg.begin, .y = -seg.begin - seg.limit, .z = seg.end, .index = i});
            }
            for (auto &e: ele)  e.x = get_index(e.x), e.y = get_index(e.y), e.z = get_index(e.z);
            auto cur = PartialOrderCounter3D(std::move(ele)).count();
            ranges::transform(ans, cur, ans.begin(), std::plus{});
            // for (auto x: cur)  std::cout << x << " ";
            // std::cout << std::endl;
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        // x1 <= x2, x2 <= y2, y2 <= y1, y2 - x2 >= K
        // 即 -x2 <= -x1 (x), x2 - y2 <= -K (y), y2 <= y1 (z)
        {
            std::vector<Element3d> ele;
            for (i32 i = 0; i < N; i++) {
                auto const &seg = segs[i];
                ele.push_back({.type = Element3d::Update, .x = -seg.begin, .y = seg.begin - seg.end, .z = seg.end});
                ele.push_back({.type = Element3d::Query, .x = -seg.begin, .y = -seg.limit, .z = seg.end, .index = i});
            }
            debug  for (auto e: ele) {
                std::cout << std::format("{} ({}, {}, {}) [{}]", e.type == Element3d::Update? 'u': 'q', e.x, e.y, e.z, e.index) << std::endl;
            }
            for (auto &e: ele)  e.x = get_index(e.x), e.y = get_index(e.y), e.z = get_index(e.z);
            auto cur = PartialOrderCounter3D(std::move(ele)).count();
            ranges::transform(ans, cur, ans.begin(), std::plus{});
            // for (auto x: cur)  std::cout << x << " ";
            // std::cout << std::endl;
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        // x2 <= x1, x1 <= y1, y1 <= y2, y1 - x1 >= K
        // 即 x2 <= x1 (y), -y2 <= -y1 (x), y1 - x1 >= K
        {
            std::vector<Element2d> ele;
            for (i32 i = 0; i < N; i++) {
                auto const &seg = segs[i];
                if (seg.end - seg.begin < seg.limit)  continue;
                ele.push_back({.type = Element2d::Update, .x = -seg.end, .y = seg.begin});
                ele.push_back({.type = Element2d::Query, .x = -seg.end, .y = seg.begin, .index = i});
            }
            for (auto &e: ele)  e.x = get_index(e.x), e.y = get_index(e.y);
            auto cur = PartialOrderCounter2D(std::move(ele)).count();
            ranges::transform(ans, cur, ans.begin(), std::plus{});
            // for (auto x: cur)  std::cout << x << " ";
            // std::cout << std::endl;
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }

        // 尝试去重
        {
            std::vector<std::pair<Segment, i32>> a;  // 线段 - 原始下标，按照线段的左端点排序
            for (i32 i = 0; i < N; i++)  a.emplace_back(segs[i], i);

            ranges::sort(a, std::less{}, lam(x, x.first.begin));

            for (auto it = a.begin(); it != a.end(); ) {
                auto [seg, index] = *it;
                auto this_begin = seg.begin;
                auto it2 = ranges::find_if(it, a.end(), lam(x, x.first.begin != this_begin));

                // [it, it2) 范围内左端点相等
                ranges::sort(it, it2, std::less{}, lam(x, x.first.end));  // 这一部分按照长度排序
                ranges::for_each(it, it2, [&](std::pair<Segment, i32> const &x) {
                    auto [seg, i] = x;
                    auto first = ranges::lower_bound(it, it2, seg.limit, std::less{}, lam(x, x.first.end - x.first.begin));
                    ans[i] -= it2 - first;
                });
                it = it2;
            }
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        {
            std::vector<std::pair<Segment, i32>> a;  // 线段 - 原始下标，按照线段的右端点排序
            for (i32 i = 0; i < N; i++)  a.emplace_back(segs[i], i);

            ranges::sort(a, std::less{}, lam(x, x.first.end));

            for (auto it = a.begin(); it != a.end(); ) {
                auto [seg, index] = *it;
                auto this_end = seg.end;
                auto it2 = ranges::find_if(it, a.end(), lam(x, x.first.end != this_end));

                // [it, it2) 范围内右端点相等
                ranges::sort(it, it2, std::greater{}, lam(x, x.first.begin));  // 这一部分按照长度排序
                ranges::for_each(it, it2, [&](std::pair<Segment, i32> const &x) {
                    auto [seg, i] = x;
                    auto first = ranges::lower_bound(it, it2, seg.limit, std::less{}, lam(x, x.first.end - x.first.begin));
                    ans[i] -= it2 - first;
                });
                it = it2;
            }
        }
        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        {
            // 再减去一次完全相同的
            std::multiset<std::pair<i32, i32>> s;
            for (auto seg: segs)  s.emplace(seg.begin, seg.end);

            for (i32 i = 0; i < N; i++) {
                ans[i] -= s.count({segs[i].begin, segs[i].end});
            }
        }

        debug { std::cout << __LINE__ << ": "; for (auto x: ans) { std::cout << x << " "; }  std::cout << std::endl; }
        // 自己还是会多算
        for (auto &x: ans)  x--;
        for (auto x: ans)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2107735140546012::solve();
    return 0;
}
