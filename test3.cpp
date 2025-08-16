#include "./libs/unicode.hpp"

using namespace unicode;
using namespace unicode_literals;

auto main() -> int {
    std::freopen("xu.in", "r", stdin);
    std::freopen("xu.out", "w", stdout);
    std::cout << "使得游戏成为平局最少需要的移动次数。"_utf8 << std::endl;
    return 0;
}
