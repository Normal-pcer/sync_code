/**
 * @link https://www.luogu.com.cn/problem/P4441
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"

#include "./libs/debug_log.hpp"

#include "./libs/rolling_container.hpp"
using namespace lib;

/*
F[i][j][k] 表示从点 (i, j) 走到游戏结束，栈的大小为 k，最优解的长度。
不仅要记录长度，还要记录
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
    class BracketString {
        uz static constexpr maxBlocks = 5;
        uz static constexpr badSize = std::numeric_limits<uz>::max();
        uz size_{};
        std::array<u64, maxBlocks> data_{};

        auto static getBlockPos(uz index) -> std::pair<uz, uz> {
            return {index >> 6, index & 63};
        }
        auto rangeCheck(uz index) const -> void {
            if (index >= size()) {
                throw std::range_error{std::format("BracketString out-of-bound access: at pos {} (which size is {})", index, size())};
            }
        }
        auto static charToBool(char ch) -> bool {
            // 0 = '('  1 = ')'
            assert(ch == '(' or ch == ')');
            return ch == ')';
        }
    public:
        BracketString() {}
        
        auto size() const -> uz { return size_; };
        auto bad() const -> bool { return size() == badSize; }
        auto static maxSize() -> uz {
            return 64 * maxBlocks;
        };
        auto getAt(uz index) const -> char {
            rangeCheck(index);
            auto [block, pos] = getBlockPos(index);
            return "()"[(data_[block] >> pos) & 1];
        };
        auto setAt(uz index, char ch) -> void {
            rangeCheck(index);
            auto target = charToBool(ch);
            auto [block, pos] = getBlockPos(index);
            auto mask = (u64)target << pos;
            data_[block] = (data_[block] & ~((u64)1 << pos)) | mask;
        }
        auto pushBack(char ch) -> void {
            assert(size_ <= maxSize());
            size_++;
            setAt(size() - 1, ch);
        }

        auto static makeBad() -> BracketString {
            BracketString res;
            res.size_ = badSize;
            return res;
        }

        // 先按照长度反向比较，再按照 **高位优先** 的字典序比较
        auto friend operator<=> (BracketString const &lhs, BracketString const &rhs) -> std::strong_ordering {
            if (lhs.bad()) return std::strong_ordering::greater;
            if (rhs.bad()) return std::strong_ordering::less;
            if (auto cmp = rhs.size() <=> lhs.size(); cmp != 0) return cmp;
            for (auto i = BracketString::maxBlocks; i --> 0; ) {
                if (auto cmp = lhs.data_[i] <=> rhs.data_[i]; cmp != 0) {
                    return cmp;
                }
            }
            return std::strong_ordering::equal;
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

        i16 maxK = (rowsCount + 1) / 2;
        auto inner = std::vector(colsCount, std::vector(maxK + 1, BracketString::makeBad()));
        RollingContainer<std::vector<decltype(inner)>, 2> F(inner);
        for (i16 col = 0; col != colsCount; col++) {
            F[0][col][0] = {};
        }
        for (i16 row = 1; row <= rowsCount; row++) {
            for (i16 col = 0; col != colsCount; col++) {
                if (mat[row][col] == '*') {
                    F[row][col][0] = {};
                }
            }
        }

        for (i16 row = 1; row <= rowsCount; row++) {
            for (i16 col = 0; col < colsCount; col++) {
                for (i16 k = 0; k <= std::min(maxK, row); k++) {
                    auto from = [&](i16 newRow, i16 newCol) -> BracketString {
                        if (newCol < 0 or newCol >= colsCount) return BracketString::makeBad();
                        auto ch = mat[newRow][newCol];

                        if (ch == '*') {
                            if (k != 0) return BracketString::makeBad();
                            else return {};
                        } else if (ch == '.') {
                            // 直接延续之前的
                            return F[newRow][newCol][k];
                        } else if (ch == '(') {
                            i16 newK = k + 1;
                            if (newK > maxK) return BracketString::makeBad();
                            auto res = F[newRow][newCol][newK];
                            if (res.bad()) return BracketString::makeBad();
                            res.pushBack(mat[newRow][newCol]);
                            return res;
                        } else if (ch == ')') {
                            i16 newK = k - 1;
                            if (newK < 0) return BracketString::makeBad();
                            auto res = F[newRow][newCol][newK];
                            if (res.bad()) return BracketString::makeBad();
                            res.pushBack(mat[newRow][newCol]);
                            return res;
                        } else {
                            assert(false), __builtin_unreachable();
                        }
                    };
                    
                    // 自定义的三路比较运算符，更小值是更希望的。
                    auto &cur = F[row][col][k];
                    std::pair<BracketString, std::pair<i32, i32>> best{};
                    best.first = BracketString::makeBad();
                    chkMin(best, {from(row - 1, col - 1), {row - 1, col - 1}});
                    chkMin(best, {from(row - 1, col), {row - 1, col}});
                    chkMin(best, {from(row - 1, col + 1), {row - 1, col + 1}});
                    cur = best.first;

                    // std::cout << std::format("{}, {}, {} <- {}, {}", row, col, k, best.second.first, best.second.second) << std::endl;
                }
            }
        }

        auto ans = F[startRow][startCol][0];
        if (ans.bad()) {
            std::cout << -1 << endl << endl;
            return;
        }
        std::cout << ans.size() << endl;

        debug for (i16 row = 1; row <= rowsCount; row++) {
            for (i16 col = 0; col < colsCount; col++) {
                for (i16 k = 0; k <= std::min(maxK, row); k++) {
                    if (F[row][col][k].bad()) continue;
                    printValues(row, col, k);
                    std::cout << "size = " << F[row][col][k].size() << ", ";
                    for (auto i = F[row][col][k].size(); i --> 0; ) {
                        std::cout << F[row][col][k].getAt(i);
                    }
                    std::cout << endl;
                }
            }
        }

        for (auto i = ans.size(); i --> 0; ) {
            std::cout << ans.getAt(i);
        }
        std::cout << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_9523125753523414::solve();
    return 0;
}
