/**
 * @link https://www.luogu.com.cn/problem/P10139
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 假设自己留下了 m 个数。
 * 第一次向里面插入元素的时机是 m，然后是 m + (m - 1), m + (m - 1) + (m - 2)...
 * 所有小于 m 的数都可以留给助手。
 * 
 * 2 5 100 1 4 5
 * 先从小到大排序：1 2 4 5 5 100
 * 如果把 1 2 4 都留给助手，此时 m = 3，剩下最小的是 5，所以 5 在第 3 秒就已经被插入了，不行；
 * 1 2 都留给助手，此时 m = 4，5 会在第 4 秒被插入。
 * 不对，不一定非要把小的分配给助手。
 * 
 * 我们先固定一个 m，比如指定 m = 3。
 * 那么 Bessie 放入元素的时间就是 3, 5, 6 这三个时刻。
 * 第 6 秒由 Bessie 放入一个 100。
 * 第 5 秒可以由助手放入一个 5，或许就让助手放？
 * 第 5 秒还可以再由 Bessie 放入一个 5。
 * 第 4 秒可以让助手放一个 4。
 * 第 3 秒可以让 Bessie 放一个 2。
 * 第 1 秒让助手放一个 1。
 * 
 * 似乎这样从大到小遍历一次，就可以让助手放尽可能多的数了。
 * 让助手拿到的数尽可能多，同时不能让助手拖 Bessie 的后腿。
 * 助手不能比 Bessie 晚结束多于 m 秒。
 * 这种限制下，再让助手多拿数大概就是最优了。
 * 
 * 可以二分判断 Bessie 最少持有多少个数。
 * 
 * 如果严格限制助手不能比 Bessie 早结束可以吗？
 * 还是比较奇怪。再想想。
 * 
 * 再试试从小到大思考。还是固定 Bessie 拿 m = 3 个数。
 * 1 2 4 5 5 100
 * Bessie 放第一个数是第 3 秒。1 和 2 一定可以安全地放置。Bessie 放第一个大于等于 3 的数即 4。
 * 接下来是 5 秒。Bessie 放一个 5。
 * 接下来是 6 秒。助手可以放一个 5，Bessie 放一个 6。
 * 
 * 
 * 感觉还是这样合理一些。
 * 突然发现似乎最后得到的答案和 a[n] 取较小值即可。
 * 
 */
namespace Solution_5063138640260486 {
    void solve_force() {
        i32 N;  std::cin >> N;
        std::vector<i64> a(N);
        for (auto &x: a)  std::cin >> x;

        using Status = u32;
        auto stat_cnt = (Status)1 << N;

        i64 ans = infLL;
        for (Status i = 0; i < stat_cnt; i++) {
            std::deque<i64> self, other;

            for (i32 j = 0; j < N; j++) {
                if (i & ((Status)1 << j))  self.push_back(a[j]);
                else  other.push_back(a[j]);
            }

            ranges::sort(self), ranges::sort(other);
            std::vector<i64> res;

            i64 t = 0;
            while (not self.empty()) {
                t += self.size();
                auto x = self.front();  self.pop_front();
                while (not other.empty() and other.front() < t)  res.push_back(other.front()), other.pop_front();
                res.push_back(x);
            }
            if (not other.empty())  t = other.back();
            ranges::copy(other, std::back_inserter(res));

            if (ranges::is_sorted(res))  chkMin(ans, t);
        }

        std::cout << ans << endl;
    }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U x, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), x)) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        return begin;
    }
    void solve() {
        i32 N;  std::cin >> N;
        std::vector<i64> a(N);
        for (auto &x: a)  std::cin >> x;
        ranges::sort(a);

        auto can_allocate = [&](i32 m) -> bool {
            i64 next_delta = m, next_time = 0;
            auto it = a.begin();
            i32 friend_count = 0;
            for (auto _ = m; _ --> 0; ) {
                next_time += next_delta, next_delta--;
                while (it != a.end() and *it < next_time)  it++, friend_count++;
                if (it == a.end())  return true;
                it++;
            }
            return it == a.end();
        };

        auto x = lowerBoundValue(0, N + 1, true, can_allocate);
        auto ans = static_cast<i64>(x) * (x + 1) / 2;
        ans = std::min(ans, a[N - 1]);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    i32 T;  std::cin >> T;
    while (T --> 0) {
        Solution_5063138640260486::solve();
    }
    return 0;
}
