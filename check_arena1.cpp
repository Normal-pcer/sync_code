#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution {
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
            i32 constexpr limit = 6;
            if (end - begin <= limit) {
                for (auto i = begin; i != end; i++) {
                    if (i->type != Element::Query) continue;
                    for (auto j = begin; j != end; j++) {
                        if (i->x < j->x) break;
                        if (j->type == Element::Update and j->y <= i->y and j->z <= i->z) {
                            ans[i->index]++;
                        }
                    }
                }
                // 插入排序，按照 y 排序。
                for (auto i = begin; i != end; i++) {
                    ranges::rotate(ranges::upper_bound(begin, i, i->y, ranges::less{}, lam(e, e.y)), i, i + 1);
                }
                return;
            }
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
    auto solve() -> void {
        i32 N, K; std::cin >> N >> K;

        using Ele3d = PartialOrderCounter3D::Element;
        std::vector<Ele3d> ele;
        ele.reserve(N + N);

        for (i32 i = 0; i < N; i++) {
            i32 x, y, z; std::cin >> x >> y >> z;
            ele.push_back({Ele3d::Update, x, y, z, i});
            ele.push_back({Ele3d::Query, x, y, z, i});
        }

        auto ans = PartialOrderCounter3D{std::move(ele)}.count();
        std::vector<i32> cnt(N + 1);
        for (auto x: ans) cnt[x]++;
        for (i32 i = 1; i <= N; i++) {
            std::cout << cnt[i] << endl;
        }
    }
}

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
}
