#include <algorithm>
#include <bits/ranges_algo.h>
#include <cstdint>
#include <ios>
#include <iostream>
#include <vector>
using i32 = std::int32_t;
namespace ranges = std::ranges;
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N, Q;  std::cin >> N >> Q;

    std::vector<i32> all_x, all_y;
    for (i32 _ = N; _ --> 0; ) {
        char ch;  std::cin >> ch;
        i32 val;  std::cin >> val;
        if (ch == 'H')  all_y.push_back(val);
        else  all_x.push_back(val);
    }

    ranges::sort(all_x);
    ranges::sort(all_y);

    for (i32 q = Q; q --> 0; ) {
        i32 x, y, val;
        std::cin >> x >> y >> val;

        enum Type {
            None,  // 无
            Street,  // 纵向
            Road,  // 横向
            Cross,  // 交叉
        };
        auto check_point = [&](i32 x, i32 y) {
            bool has_st = false, has_rd = false;
            if (ranges::binary_search(all_x, x))  has_st = true;
            if (ranges::binary_search(all_y, y))  has_rd = true;
            return static_cast<Type>(has_st | has_rd << 1);
        };
        for (auto t = 0; t != val; t++) {
            auto type = check_point(x, y);
            if (type == Street or (type == Cross and t % 2 == 0)) {
                y++;
            } else {
                x++;
            }
        }
        std::cout << x << " " << y << std::endl;
    }
}
