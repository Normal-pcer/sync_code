#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;


auto solve() -> void {
    i32 N;  std::cin >> N;
    std::vector<std::tuple<i32, i32, i32, i32>> vec(N);

    for (auto &[a, b, c, d]: vec)  std::cin >> a >> b >> c >> d;

    i64 size = 0;
    for (auto x: vec)  size += std::get<0>(x);

    i64 min = -1;
    i64 mid_index = (size + 1) / 2 - 1;
    i64 cur_index = 0;
    std::sort(vec.begin(), vec.end(), lam(x, y, std::get<2>(x) < std::get<2>(y)));
    for (auto &[a, b, c, d]: vec) {
        cur_index += a;
        if (cur_index > mid_index) {
            min = c;
            break;
        }
    }

    i64 max = -1;
    mid_index = size - mid_index - 1;
    cur_index = 0;
    std::sort(vec.begin(), vec.end(), lam(x, y, std::get<3>(x) > std::get<3>(y)));

    for (auto &[a, b, c, d]: vec) {
        cur_index += a;
        if (cur_index > mid_index) {
            max = d;
            break;
        }
    }

    std::cout << mid_index << " " << min << " " << max - min + 1 << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 C, T;  std::cin >> C >> T;
    if (C % 2 == 0)  return -1;
    while (T --> 0)  solve();
}