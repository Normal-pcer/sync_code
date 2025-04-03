/**
 * @link https://www.luogu.com.cn/problem/AT_abc399_e
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
先预处理一下谁希望被替换成谁。
正常来讲，a -> b，b -> c 这样的替换必须先 b -> c 再 a -> b。
那么做一遍拓扑排序就好了。
如果遇到环怎么办？例如 
a -> b
b -> c
c -> a

这时可以借助一个临时的 tmp
c -> a (1)
a -> tmp (2)
tmp -> b (3)
b -> c (4)

连边：
2 1
1 4
4 3
2 3
拓扑序：
2 1 4 3

也就是，遇到一个环的时候可以考虑断一条边，1 -> 2 变为 1 -> 3 <- 2
这个点完全与图无关当然是最好的。如果不存在，应该怎么分配？
似乎没有空点一定解不开环？
拿非空的点解环似乎完全不可行。
另一方面，只要有一个空点，拿它解掉任意多环都是可行的，因为只有入边没有出边。

*/
namespace Solution_6393679346026370 {
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_6393679346026370::solve();
    return 0;
}
