/**
 * 
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_2295588826700560 {
    bool constexpr noQuery = false;

    i32 N, M, K;
    std::mt19937 rng{std::random_device{}()};
    std::vector<i32> a;

    auto randomModify() -> std::pair<i32, i32> {
        i32 x = rng() % N + 1, y = rng() % N + 1;
        i32 del = rng() % 10 - 5;
        
        for (i32 i = x; i <= y; i++)  a[i] += del;
        std::cout << std::format("* Modify [{}, {}]: {}", x, y, del) << std::endl;
        for (i32 i = 1; i <= N; i++)  std::cout << a[i] << " ";
        std::cout << std::endl;
        return {x, y};
    }
    auto handleQuery(i32 x, i32 y) -> i32 {
        return a[x] >= a[y];
    }
    auto checkAns(i32 x) -> void {
        auto correct = std::ranges::max(a | views::drop(1)) == a[x];
        if (not correct)  std::cout << "WA" << std::endl, std::exit(0);
    }

    i32 _cnt;
    auto queryLess(i32 x, i32 y) -> bool {
        if (x == 0)  return true;
        if (y == 0)  return false;
        // _cnt--, assert(_cnt > 0);
        _cnt--;
        if (_cnt <= 0)  return rng() & 1;
        std::cout << std::format("? {} {}", x, y) << std::endl;
        if constexpr (noQuery) {
            return handleQuery(x, y) == 0;
        } else {
            i32 res;  std::cin >> res;
            return res == 0;
        }
    }
    auto answer(i32 x) -> void {
        std::cout << std::format("! {}", x) << std::endl;
        if constexpr(noQuery)  checkAns(x);
    }
    class SegTree {
        struct Node {
            int begin, end;
            int max_element;  // 这个区间内可能的最大元素
        };
        std::vector<Node> tree;
        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }
        auto pushUp(i32 p) -> void {
            tree[p].max_element = std::max(tree[lson(p)].max_element, tree[rson(p)].max_element, queryLess);
        }
        auto build(i32 begin, i32 end, i32 p = 1) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(i32 N): tree((N + 1) << 2) {
            build(1, N+1, 1);
        }

        auto queryAll() -> i32 {
            i32 constexpr root = 1;
            return tree[root].max_element;
        }
        auto updateAt(i32 val, i32 pos, i32 p = 1) -> void {
            if (tree[p].begin >= pos and tree[p].end <= pos + 1) {
                tree[p].max_element = val;
                return;
            }
            if (tree[lson(p)].end > pos)  updateAt(val, pos, lson(p));
            else  updateAt(val, pos, rson(p));
            pushUp(p);
        }
    };
    void solve() {
        std::cin >> N >> M >> K;
        if constexpr(noQuery)  a.resize(N+1), std::fill(a.begin(), a.end(), 0);

        SegTree sgt{N};
        _cnt = K;
        sgt.updateAt(N, N);
        while (M --> 0) {
            i32 l, r;
            if constexpr(noQuery) {
                std::tie(l, r) = randomModify();
            } else {
                std::cin >> l >> r;
            }
            _cnt = K;

            // l - 1 可能成为新的最大值（区间降低）
            // r 可能成为新的最大值（区间提升）
            if (l - 1 != 0)  sgt.updateAt(l - 1, l - 1);
            sgt.updateAt(r, r);
            auto ans = sgt.queryAll();
            answer(ans);
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    i32 constexpr T = 1;
    for (i32 _ = 0; _ < T; _++) {
        Solution_2295588826700560::solve();
    }
    return 0;
}
