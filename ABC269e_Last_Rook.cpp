/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_e
 */

#include "./lib_v3.hpp"

using namespace lib;

/**
 * 确定一行一列没有棋子
 * 以 8 行 8 列为例
 * 如果前 4 行中有答案，那么后 4 行应该有 4 个棋子
 * 如果答案在 3~4 行，那么 1~2 行应该有 2 个棋子
 * 行列分别二分即可。
 */
namespace Solution_9568530168324347 {
    void solve() {
        int N;  std::cin >> N;

        // query：查询区间 [x, y) 内的个数
        auto find = [&](auto &&query) {
            auto begin = 0, end = N;
            while (begin + 1 < end) {
                auto mid = std::midpoint(begin, end);
                // 查询 [begin, mid) 行的全部格子
                // ;
                query(begin, mid);
                int cnt;  std::cin >> cnt;
                
                auto full_cnt = mid - begin;
                assert(cnt == full_cnt or cnt == full_cnt - 1);
                if (cnt == full_cnt) {
                    // 答案一定不在 [begin, mid)
                    begin = mid;
                } else {
                    end = mid;
                }
            }
            return begin;
        };

        // 二分查找哪行
        auto row = find([&](int begin, int mid) -> void { std::cout << "? " << begin + 1 << " " << mid << " " << 1 << " " << N << std::endl; }) + 1;
        // std::cout << "row " << row << std::endl;

        // 同样地，查询列
        auto col = find([&](int begin, int mid) -> void { std::cout << "? " << 1 << " " << N << " " << begin + 1 << " " << mid << std::endl; }) + 1;
        // std::cout << "col " << col << std::endl;
        std::cout << "! " << row << " " << col << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9568530168324347::solve();
    return 0;
}
