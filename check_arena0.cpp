/**
 * @link https://www.luogu.com.cn/problem/P8010
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

#include "./libs/io.hpp"

using namespace lib;

/**
 * A(l, r) 的意义：区间最大值最靠右的出现位置
 * 先考虑固定右端点，向左延伸的区间。在左端点向左移动的过程中，单调不减。
 * 固定右端点 i，min A(j, i) = A(1, i), max A(j, i) = A(i, i) = i
 * 固定左端点 1，A(1, i) 一定单调不减
 * max A(1, i) 即 A(1, r)
 * min A(i, i) 即 l
 * 
 * Omega(l, r) = min{|i - A(1, j)|}（[i, j] 为子区间)
 * l 一定是这一堆 i 里的最小值
 * A(1, r) 是最大的，可能小于或大于 l
 * 如果 A(1, r) < l 那么 l - A(1, r) 是最优解
 * 否则一定有一个只有单元素的区间，即 0
 * Omega(l, r) = max(0, l - A(1, r))
 * 
 * 只需考虑如何维护 A(1, r)
 * 注意到 A(1, x) 一定是单调不减的，同时一次修改会让若干个数只增不减。
 * 一段连续区间一定会有同样的 A(1, r)，修改可能会分割区间。
 * 反过来考虑，可以把上述这些区间内部连接起来。
 * 进行一次区间减法，可能会进行连续区间的合并。
 * 具体地，如果这次减法波及到了几个区间的开头，让它们低于了前面的某些值，可以把它合并到前面
 * 对于每一个点，可以查询它所在区间的范围，以及这个区间的最大高度。
 * 尝试使用并查集。
 */
namespace Solution_5128899434622973 {
    struct Range {
        i32 begin, end;
    };  // 左闭右开区间
    struct DSU {
        std::vector<i32> fa;

        DSU(i32 N): fa(N+1) {
            for (i32 i = 1; i <= N; i++) {
                fa[i] = i;
            }
        }
        auto find(i32 x) -> i32 {
            if (fa[x] == x)  return fa[x];
            return fa[x] = find(fa[x]);
        }
        auto connect(i32 x, i32 y) -> void {
            auto a = find(x);
            auto b = find(y);
            fa[b] = a;
        }
    };
#define cin kobe
#define cout bryant
    void solve() {
        i32 N, Q;  io >> N >> Q;

        std::vector<u32> init(N);
        for (auto &x: init)  io >> x;

        std::vector<std::tuple<i32, i32, i32>> queries(Q);
        for (auto &[op, x, val]: queries) {
            io >> op >> x >> val, x--;
            if (op == 2)  val--;
        }

        DSU dsu{N};
        std::vector<i64> tops(N);
        std::vector<i32> next(N, -1);
        {
            i64 prev = -1;
            i64 prev_max = -infLL;
            for (i32 i = 0; i < N; i++) {
                auto cur = init[i];
                if (cur >= prev_max) {
                    prev_max = cur;
                    tops[i] = cur;
                    if (prev != -1)  next[prev] = i;
                    prev = i;
                } else {
                    assert(i != 0);
                    dsu.connect(i - 1, i);
                }
            }
        }

        // 对 tops[i] 差分
        {
            auto prev = tops[0];
            for (auto i = next[0]; i != -1; prev += tops[i], i = next[i]) {
                tops[i] = tops[i] - prev;
            }
        }

        for (auto [op, x, val]: queries) {
            if (op == 1) {
                auto a = dsu.find(x);

                for (i32 i = next[a], p = a; i != -1; i = next[i]) {
                    auto j = next[i];

                    tops[i] -= val;
                    if (tops[i] < 0) {
                        dsu.connect(a, i);
                        if (j != -1)  tops[j] += tops[i] + val;
                        next[p] = j;
                    } else {
                        break;
                    }
                }
            } else {
                auto l = x, r = val;
                // 求 A(1, r)
                auto a_1_r = dsu.find(r);
                debug  io << std::format("A(1, r) = {}", a_1_r) << endl;
                auto ans = std::max<i64>(0, l - a_1_r);
                io << ans << endl;
            }
        }
    }
#undef cout
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5128899434622973::solve();
    return 0;
}
