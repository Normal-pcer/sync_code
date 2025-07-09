/**
 * @link
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
using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::uint16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
using namespace lib;

namespace Solution_5017925738629204 {
    using Mass = double;  // 0.5 为单位，不会有精度误差，随便用

    std::map<std::string_view, Mass> const elements{
        {"H", 1},
        {"C", 12},
        {"N", 14},
        {"O", 16},
        {"F", 19},
        {"Na", 23},
        {"Mg", 24},
        {"Al", 27},
        {"Si", 28},
        {"P", 31},
        {"S", 32},
        {"Cl", 35.5},
        {"K", 39},
        {"Ca", 40},
        {"Mn", 55},
        {"Fe", 56},
        {"Cu", 64},
        {"Zn", 65},
        {"Ag", 108},
        {"I", 127},
        {"Ba", 137},
        {"Hf", 178.5},
        {"Pt", 195},
        {"Au", 197},
        {"Hg", 201}
    };

    /**
     * 从给定的范围中，从开头处解析一个元素名称。
     * 返回子范围的尾后迭代器。
     */
    template <typename Iter>
    auto parseElement(Iter first, Iter last) -> Iter {
        assert(first != last and std::isupper(*first));
        auto it = std::next(first);
        // 读取一系列小写字母
        for (; it != last; ++it) {
            if (not std::islower(*it)) break;
        }
        return it;
    }

    /**
     * 计算一个化学式的相对分子质量。
     */
    auto calc(std::string_view str) -> Mass {
        auto it = str.begin();
        Mass mass{0};

        
        while (it != str.end()) {
            auto elementMass = [&]() -> Mass {
                // 尝试读取原子团
                if (*it == '(') {
                    auto endBracket = ranges::find(it, str.end(), ')');
                    assert(endBracket != str.end());
                    auto result = calc({std::next(it), endBracket});
                    it = std::next(endBracket);
                    return result;
                } else {
                    auto elementEnd = parseElement(it, str.end());
                    auto result = elements.at({it, elementEnd});
                    it = elementEnd;
                    return result;
                }
            }();

            // 尝试读取元素数量
            i32 count{1};
            if (it != str.end() and *it == '_') {
                ++it, ++it;  // 指针恰好指向数字
                // 解析十进制数字
                count = 0;
                for (; it != str.end() and std::isdigit(*it); ++it) {
                    count = count * 10 + (*it - '0');
                }
                ++it;
            }

            mass += elementMass * count;
        }

        return mass;
    }

    auto solve() -> void {
        std::string expr{};
        std::cin >> expr;

        Mass mass{0};

        // 特殊处理结晶水
        if (auto it = ranges::find(expr, '~'); it != expr.end()) {
            auto first = it; // 备份波浪线位置
            ++it; // 指向数字开头
            i32 count{1};
            if (it != expr.end() and std::isdigit(*it)) {
                count = 0;
                for (; it != expr.end() and std::isdigit(*it); ++it) {
                    count = count * 10 + (*it - '0');
                }
            }

            mass += calc({it, expr.end()}) * count;
            expr.erase(first, expr.end());
        }

        mass += calc(expr);

        std::cout << mass << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5017925738629204::solve();
    return 0;
}
