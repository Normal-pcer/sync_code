/**
 * @link https://www.luogu.com.cn/problem/P10280
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr(0)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
using ll = long long; using ull = unsigned long long;
int constexpr inf = 0x3f3f3f3f;  long long constexpr intLL = 0x3f3f3f3f3f3f3f3fLL;  char constexpr endl = '\n';

#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 10 00 00 00 00
 * __ __ __ __ *
 * 
 * 每次最多交换距离为 K，那么把一个 1 移动到距离为 N 的位置上，需要的交换次数是 N / K 向上取整。
 * 注意到，如果这个 1 和目标之间有 1，那么似乎可以把中间的那个 1 移动到目标点，自己移动到那个位置。
 * 不过不管怎么样，这么多步骤之内都可以实现和远处的位置交换。
 * 
 * 接下来，凭感觉，让交换前后所有 1 的相对顺序不变是比较优的方法。
 * 如果这个策略成立，找出所有的 1 直接对应地移动即可。
 * 先随便写一下，反正很好写。
 * 
 * 这样可以很轻易地举出反例：
 * N = 3, K = 2
 * 110
 * 011
 * 
 * 让第一个 1 移动距离 2 即可。
 * 可以发现，这两步都存在一定的“浪费”——它们本来都可以移动 2 格远，但是都只有 1 格。
 * 考虑一个新的策略：让每一次移动尽可能凑整。
 * 那么似乎相对顺序并不是特别重要。
 * 并非不重要。只考虑凑整，可能会弄出这样的东西：
 * A000000B0
 * 0b0000a00
 * 
 * 限制一下，为了凑整也不要增加更多步数。
 * 先跑一遍原先的方法记录步数上限，然后使用一个 set 维护所有点，每次查询最接近步数 * 步长的点删除。
 * 
 * 然而并不是完全有道理。想一想反例。
 * 9 3
 * 100001100
 * 000001110
 * 
 * 第一个 1 稍微多移动一点，可以让两个人都不用干活了。
 * 其实这样正常也可以。可以让一个人完全不用干活，另一个人多一步。总体也是只多一步。
 * 感觉更加合理的应该是每次找到最小的剩余，然后在此基础上尽可能凑整。
 * 通过了这组数据。
 * 
 * 怎么还是 24 分。
 * 3 3
 * 101
 * 011
 * 
 * 似乎把已经匹配上的删去就可以了。
 * 为了通过率，先检查一下。
 * 写个对拍。另一份代码写一个状压。
 * 
 * 还是不对。
 * 考虑把所有的不匹配先处理出来，这些不匹配的再 0-1 配对。
 * 忍不了了。看题解。
* 
 * 依次遍历所有的 unmatch，按照 0/1 分类。这一步似乎想得很对。
 * 接下来，如果可以，一定和模 K 同余的数配对。其次是差 1，差 2……
 * 特别地，如果暂时还没有对应的另一种，直接存下来。
 * 
 * 这个有道理吗？
 * 首先猜测一个结论：每个 1 和自己移动后位置进行连线，这些连线如果有交叉一定不可能更优。
 * 这是对的吗？
 * 设 first 上的几个点叫做 a1, a2；last 上的几个点叫做 b1, b2.
 * 这里先假设 a1 < b1 < b2 < a2
 * 如果没有交点：
 * A = ceil((b1 - a1) / K) + ceil((a2 - b2) / K)
 * 如果有交点：
 * B = ceil((b2 - a1) / K) + ceil((a2 - b1) / K)
 * 希望 A <= B。
 * 
 * 这个显然是正确的。
 * 似乎其他的情况也都是同理。
 * 并非同理。这个结论对于其他的情况就不成立了。
 * 
 * 
 * 一点都不会啊。
 * 首先，先假设 K = 1，此时每个 unmatch 都和前面的尽可能匹配。因为对于前面一个没有配对过的元素，与其等到以后配对，不如现在就配上。
 * 例如：
 * 00....1.(1)
 * ||____|
 * |_______
 * 如果这个 1 等着和后面配对
 * 00....1.1....0...1
 * ||____| |____|   |
 * |________________|
 * 但是如果当场就配对上
 * 00....1.1....0...1
 * ||____| |    |===|
 * |_______|
 * 
 * 那么上面的大段一定会严格长于标等号的一段，一定是比较亏的。
 * 
 * 而且此时总共的交换距离一定是定值。例如：
 * 0....0..1........1
 * |____|__|        |
 *      |___________|
 * 
 * 0....0..1........1
 * |    |__|        |
 * |________________|
 * 
 * 但是事实上 K 不等于 1。假设要交换 x 和 y 两个点（x < y），相当于要多走一部分距离。
 * （也就是说，这段距离本来可以留给实际走，但是被白白浪费了）
 * 这个额外距离是 (K - (y - x) % K) % K。
 * 
 * 希望额外距离尽可能小，也就是 (y - x) 尽可能接近 K 的倍数（也许可以略小于 K 的倍数）
 * 
 * 那么有没有可能进一步地减小浪费的距离，但是代价是增加总距离？
 * 从直觉上来看，浪费的距离这点蚊子腿还是不足以抵消总距离增加的代价。
 * 
 * A B C D
 * 0.1.0.1
 * |_| |_|
 * 
 * 0.1.0.1
 * | |_| |
 * |_____|
 * 
 * 这个过程中，总距离的增加量是 2|BC|。
 * 希望尽可能地减小浪费距离，那么 |BC| 和 |AD| 应该尽可能接近 K 的倍数，|AB| 和 |CD| 应该尽可能略大于 K 的倍数。
 * 随着 |BC| 从 1 增长到 K，浪费的长度减少 1，但是总长度增加了 1，二者恰好抵消了。|BC| 继续增长显然更劣。
 * 再来观察 |AD| 带来的影响。
 * |AD| mod K = |AB| mod K + |BC| mod K + |CD| mod K
 * 那么它浪费的长度相当于三者之和对 K 取模。
 * 带来的收益：
 * delta    = (-|AB|) mod K + (-|CD| mod K) - (-|AD| mod K) - |BC|
 *          = -|AB| % K - |CD| % K + (|AB| % K + |BC| % K + |CD| % K) % K - |BC|
 * 这个东西显然不会小于 0。只有这些数和 K 相比都较小的时候才会等于 0。
 * 
 * 于是得出以下正确的贪心策略：
 * 1. 对于一个未匹配的点，如果能与之前的点配对，一定进行配对。
 * 2. 如果一个点能与之前的很多点配对，让它们的距离模 K 优先为 0，-1，-2，...
 * 
 * 考虑如何实现。
 * 设一个还没有完成配对的点位置为 p。
 * 对这个点记录 p % K 和 p 形成的对偶（pair）。（不是这个名字好好笑）
 * 使用集合（set）记录。在位置为 q 的点，再寻找前面有没有 p % K >= q 的点即可。
 */
namespace Solution_4530564892107802 {
    auto div_ceil(i32 x, i32 y) -> i32 {
        return x / y + (x % y != 0);
    }
    void solve_force() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        using Status = u32;
        i32 N, K;  std::cin >> N >> K;
        std::string first, last;
        std::cin >> first >> last;

        Status stat_cnt = (Status)1 << N;
        std::vector F(stat_cnt, inf);
        
        Status first_stat = 0, last_stat = 0;
        for (i32 i = 0; i < N; i++) {
            first_stat = (first_stat << 1) | (first[i] == '1');
            last_stat = (last_stat << 1) | (last[i] == '1');
        }
        std::vector<char> vis(stat_cnt, false);
        std::deque<Status> q;
        q.push_back(first_stat);
        F[first_stat] = 0;
        while (not q.empty()) {
            auto x = q.front();  q.pop_front();
            if (vis[x])  continue;
            if (x == last_stat)  break;
            vis[x] = true;
            for (i32 i = 0; i < N; i++) {
                for (i32 j = i + 1; j < N; j++) {
                    if (j - i <= K) {
                        auto xi = x >> i & 1;
                        auto xj = x >> j & 1;
                        std::swap(xi, xj);
                        auto nx = x;
                        nx = (nx & ~((Status)1 << i)) | xi << i;
                        nx = (nx & ~((Status)1 << j)) | xj << j;
                        q.push_back(nx);
                        chkMin(F[nx], F[x] + 1);
                    }
                }
            }
        }
        auto ans = F[last_stat];
        std::cout << ans << endl;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, K;  std::cin >> N >> K;
        std::string first, last;  std::cin >> first >> last;
        using UnmatchedPoint = std::pair<i32, bool>;  // 位置，类型
        std::vector<UnmatchedPoint> unmatched;  // 两种未匹配的点

        for (i32 i = 0; i < N; i++) {
            if (first[i] == '1' and last[i] == '0')  unmatched.push_back({i, 1});
            if (first[i] == '0' and last[i] == '1')  unmatched.push_back({i, 0});
        }

        using PointFeature = std::pair<i32, i32>;  // p % K, p
        std::array<std::set<PointFeature>, 2> set;

        i64 ans = 0;
        for (auto [q, type]: unmatched) {
            auto other_type = !type;
            if (set[other_type].empty()) {
                set[type].insert({q % K, q});
                continue;
            }
            auto prev_it = set[other_type].lower_bound({q % K, 0});
            if (prev_it == set[other_type].end())  prev_it = set[other_type].begin();

            auto [_, p] = *prev_it;
            ans += div_ceil(q - p, K);

            set[other_type].erase(prev_it);
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4530564892107802::solve();
    return 0;
}
