/**
 * @link https://www.luogu.com.cn/problem/P2318
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
using namespace lib;

namespace Solution_4437874857205667 {
    using Time = i32;
    struct MemoryPage {
        i32     pageIndex{}; // 页面编号
        Time    lastLoad{}; // 上次加载进内存的时间
        i32     useCount{}; // 使用次数
        bool    isActive{}; // 现在是否在内存中

        // lhs > rhs：更加优先被封存
        auto friend operator<=> (MemoryPage const &lhs, MemoryPage const &rhs) -> std::strong_ordering {
            if (auto res = rhs.useCount <=> lhs.useCount; res != 0) return res; // useCount 小的
            else if (auto res = rhs.lastLoad <=> lhs.lastLoad; res != 0) return res; // lastLoad 小的
            else return rhs.pageIndex <=> lhs.pageIndex;
        }
    };
    // 主要逻辑
    class MemoryManager {
        Time    current{}; // 当前时间戳
        i32     limit{}; // 至多有几个活跃页面

        std::multiset<MemoryPage> active;
        std::map<i32, std::multiset<MemoryPage>::iterator> indexToActive;
        std::map<i32, MemoryPage> storaged;

        auto loadPage(i32 index, bool resetCount = true) -> void {
            assert(not indexToActive.contains(index));
            MemoryPage item{index};
            if (storaged.contains(index)) {
                item = storaged[index];
                storaged.erase(index);
            }
            if (resetCount) item.useCount = 1;
            else ++item.useCount;
            item.lastLoad = current;

            auto it = active.emplace(item);
            indexToActive.emplace(index, it);
        }
        auto storePage(i32 index) -> void {
            assert(not storaged.contains(index));
            assert(indexToActive.contains(index));
            
            auto it = indexToActive[index];
            auto item = *it;
            indexToActive.erase(index), active.erase(it);
            storaged.emplace(item.pageIndex, item);
        }
        auto refreshPage(i32 index) -> void {
            storePage(index), loadPage(index, false);
        }
    public:
        MemoryManager() {}
        MemoryManager(i32 limit): limit(limit) {}

        auto tick() -> void { ++current; }

        enum class AccessResult {
            Direct /*直接成功*/, Other /*其他方式*/
        };
        auto access(i32 index) -> AccessResult {
            if (indexToActive.contains(index)) {  // 在内存中
                refreshPage(index);
                return AccessResult::Direct;
            }
            if (static_cast<i32>(active.size()) >= limit) { // 内存满了
                // 删除一个
                auto selected = *active.rbegin();
                storePage(selected.pageIndex);
            }
            loadPage(index);
            return AccessResult::Other;
        }
    };
    auto solve() -> void {
        i32 n{}, m{};
        std::cin >> n >> m;

        MemoryManager mgr{n};
        i32 successCount{};
        for (auto _ = m; _ --> 0; ) {
            i32 x{};
            std::cin >> x;

            auto res = mgr.access(x);
            if (res == MemoryManager::AccessResult::Direct) {
                ++successCount;
                // std::cerr << "!";
            }

            mgr.tick();
        }

        std::cout << successCount << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4437874857205667::solve();
    return 0;
}
