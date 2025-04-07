/**
 * @link https://www.luogu.com.cn/problem/P4441
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

/*
F[i][j][k] 表示从点 (i, j) 走到游戏结束，栈的大小为 k，最优解的长度。
*/
namespace Solution_9523125753523414 {
    template <typename Value, typename Source>
    struct MaxItem {
        Value value;
        Source source;

        auto friend operator<=> (MaxItem const &lhs, MaxItem const &rhs) -> std::weak_ordering {
            return lhs.value <=> rhs.value;
        }
    };
    auto solve() -> void {
        i16 rowsCount, colsCount;
        std::cin >> rowsCount >> colsCount;
        
        std::vector<std::string> mat(rowsCount + 1);
        for (i32 row = 1; row <= rowsCount; row++) {
            mat[row].reserve(colsCount);
            std::cin >> mat[row];
        }
        for (auto _ = colsCount; _ --> 0; ) mat[0].push_back('*');

        i32 startRow = rowsCount;
        i32 startCol = [&]() {
            auto found = mat[startRow].find('M');
            assert(found != std::string::npos);
            return found;
        }();

        // 最优解信息
        struct SolutionInfo {
            i16 len;  // 括号序列长度
            char ch;  // 当前点的字符

            auto operator<=> (SolutionInfo rhs) const -> std::strong_ordering {
                if (auto cmp = len <=> rhs.len; cmp != 0) return cmp;
                else return rhs.ch <=> ch;
            }
        };
        struct Point {
            i16 x = -1, y = -1, z = -1;

            auto empty() const -> bool { 
                return x == -1 and y == -1 and z == -1;
            }
        };

        std::vector F(rowsCount + 1, std::vector(colsCount, std::vector(rowsCount + 2, MaxItem{(i16)-inf, Point{}})));
        for (i32 col = 0; col != colsCount; col++) {
            F[0][col][0] = {0, {}};
        }
        for (i32 row = 1; row <= rowsCount; row++) {
            for (i32 col = 0; col != colsCount; col++) {
                if (mat[row][col] == '*') {
                    F[row][col][0] = {0, {}};
                }
            }
        }

        for (i16 row = 1; row <= rowsCount; row++) {
            for (i16 col = 0; col < colsCount; col++) {
                for (i16 k = 0; k <= rowsCount; k++) {
                    SolutionInfo constexpr noneSolutionInfo{-inf, {}};
                    auto from = [&](i16 newRow, i16 newCol) -> MaxItem<SolutionInfo, Point> {
                        if (newCol < 0 or newCol >= colsCount) return {noneSolutionInfo, {}};
                        auto ch = mat[row][col];
                        auto newK = [&]() -> i16 {
                            switch (ch) {
                            case '(': return k + 1;
                            case ')': return k - 1;
                            case '.': return k;
                            case 'M': return k;
                            default: return -1;
                            }
                        }();
                        if (newK < 0) return {noneSolutionInfo, {}};

                        i16 res = F[newRow][newCol][newK].value;
                        if (res == -inf) return {noneSolutionInfo, {}};
                        if (ch != '.') res++;
                        return {res, mat[newRow][newCol], {newRow, newCol, newK}};
                    };
                    MaxItem<SolutionInfo, Point> best{{F[row][col][k].value, '_'}, {F[row][col][k].source}};
                    chkMax(best, from(row - 1, col - 1));
                    chkMax(best, from(row - 1, col));
                    chkMax(best, from(row - 1, col + 1));

                    F[row][col][k] = {best.value.len, best.source};
                }
            }
        }

        for (i16 row = 1; row <= rowsCount; row++) {
            for (i16 col = 0; col < colsCount; col++) {
                for (i16 k = 0; k <= rowsCount; k++) {
                    auto cur = F[row][col][k];
                    if (cur.value == -inf) continue;
                    // printValues(row, col, k, cur.value, cur.source.x, cur.source.y, cur.source.z);
                }
            }
        }

        auto cur = F[startRow][startCol][0];
        std::cout << cur.value - 1 << endl;

        std::string s;
        while (not cur.source.empty()) {
            auto [i, j, k] = cur.source;
            if (mat[i][j] != '.') {
                s += mat[i][j];
            }
            cur = F[i][j][k];
        }
        s.pop_back();
        std::cout << s << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_9523125753523414::solve();
    return 0;
}
