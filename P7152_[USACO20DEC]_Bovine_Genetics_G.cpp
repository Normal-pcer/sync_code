/**
 * @link https://www.luogu.com.cn/problem/P7152
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 最开始的操作是把所有连续段切开：
 * AGGCTTT
 * AG GCT T T
 * GA TCG T T
 * 
 * 那么反转后的两段有什么特点？第一段的开头和第二段的结尾一定是相同的。一段内部一定不存在连续字符。
 * 来看如下的样例：
 * GAT?GTT
 * G 一定作为第一段的开头，第二段的结尾一定是 G。
 * 那么前两段一定是在 GAT?G
 * G | AT?G，在后面不存在 A
 * GA | T?G 需要和 T 匹配，那么就应该是 GA | T?G | T | T
 * 问号不能是 TG，只能是 AC
 * GAT | ?G 后面只有 T，? 取 T 即为 GAT | TG | T | T
 * 所以是三种可能的开始序列。
 * 
 * 研究整个串中的一个子串会不会有意义？例如用 F[i][ch] 表示前 i 个字符中，最后一段以 ch 开头的方案数？
 * 其实好像还可以，我们在每一个断点处划分出一个新的段，这个段希望前一个段以特定字符开头。
 * 那么可以想出下面的一个比较暴力的方法：
 * 首先枚举断点 i，希望在这个断点处切割。
 * 然后枚举区间长度 len。枚举的同时算出这个区间的所有问号有多少种取值。
 * 具体地，一个问号不能与前后的字符相同。如果是 A ? A 那么有三种可能，A ? B 有两种可能。
 * 考虑 [1, i] [i+1, i+len] 两个区间。
 * 如果 i + len 处是常规字符，它会希望前一个段以 s[i+len] 开头。即
 * F[i+len][s[i+1]] <- F[i][s[i+len]]
 * 如果 i + len 处是问号，它会接受前一个段以任意字符开头，除非和 i + len - 1 处相同。即
 * F[i+len][s[i+1]] <- F[i][*]
 * 如果 s[i+1] 是问号，可能需要特殊处理。
 * 
 * 这个循环顺序好像有点奇怪。先枚举长度，再枚举最后一段或许会好一些。
 * 然而这个是 O(N^2)，考虑优化。
 * 
 * 查看题解得知了这样的状态：
 * F[i][j][k][l] 表示前 i 个字符，最后一段结尾开头分别为 j 和 k，倒数第二段开头为 l。此时的方案数。
 * 这样设计状态有什么好处？
 * 上面的方法是在分段时枚举最后一段的长度，但其实可以对于每一个字符，考虑将其单开一段或者拼接到上一段。
 * 一个字符可以拼到前一段上，要求它不和当前最后一段的结尾相同。
 * 一个字符可以单开一段，需要现在的最后一段已经可以满足条件，即最后一段结尾等于倒数第二段开头。
 * 最后一段的开头好像没什么用？
 * 
 * 先试着把 k 删掉。
 * G[i][j][l] 表示前 i 个字符，倒数第二段开头为 l，最后一段结尾为 j 的方案数。
 * 遇到一个普通字符：
 * 试着把它拼到前一个上面。F[i][s[i]][l] <- F[i][j][l]，只需 s[i] != j
 * 试着让它单开一段。F[i][ 点击输入文字
 * 
 * 不对，添加新的一段，最后一段的开头就变成了倒数第二段的开头。这样是必须的。
 * F[i][j][k][l] 表示前 i 个字符，最后一段结尾开头分别为 j 和 k，倒数第二段开头为 l。此时的方案数。
 * 普通字符：
 * F[i][s[i]][k][l] <- F[i-1][j][k][l]，要求 s[i] != j
 * F[i][s[i]][s[i]][k] <- F[i-1][j][k][l]，要求 l = j
 * 
 * 问号：
 * 当成四种字符分别做一遍似乎就行
 * 
 * 
 * 还要考虑一下初始状态。
 * 另外只有一段的时候似乎会有问题。
 * 第一个字符为 s1
 * F[1][s1][s1][*] = 1 行吗？试一下。
 */
namespace Solution_7378230014817902 {
    i32 constexpr mod = 1e9 + 7;
    std::string constexpr letters = "ACGT";
    auto charToIndex(char ch) -> i32 {
        switch (ch) {
        case 'A':  return 0;
        case 'C':  return 1;
        case 'G':  return 2;
        case 'T':  return 3;
        default:   return (assert(false), __builtin_unreachable(), 4);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::string s;  std::cin >> s;
        i32 N = s.size();
        s.insert(s.begin(), '_');  // 从 1 开始存

        using Inner = std::array<std::array<std::array<i32, 4>, 4>, 4>;
        std::vector<Inner> F(N+1);  // F[i][j][k][l] 
        auto init = [&](char s1) -> void {
            for (i32 l = 0; l < 4; l++) {
                F[1][s1][s1][l] = 1;
            }
        };
        if (s[1] == '?') {
            for (i32 i = 0; i < 4; i++)  init(i);
        } else {
            init(charToIndex(s[1]));
        }
        for (i32 i = 2; i <= N; i++) {
            // 认为当前 s[i] 为给定字符，进行转移
            // 这是为了方便处理问号
            auto transform = [&](char si) -> void {
                for (i32 j = 0; j < 4; j++) {
                    for (i32 k = 0; k < 4; k++) {
                        for (i32 l = 0; l < 4; l++) {
                            if (si != j) {
                                F[i][si][k][l] = (F[i][si][k][l] + F[i-1][j][k][l]) % mod;
                            }
                            if (l == j) {
                                F[i][si][si][k] = (F[i][si][si][k] + F[i-1][j][k][l]) % mod;
                            }
                        }
                    }
                }
            };

            if (s[i] == '?') {
                for (i32 i = 0; i < 4; i++)  transform(i);
            } else {
                transform(charToIndex(s[i]));
            }
        }

        i32 ans = 0;
        for (i32 j = 0; j < 4; j++) {
            for (i32 k = 0; k < 4; k++) {
                // 统计答案
                // 只需 j = l
                ans = (ans + F[N][j][k][j]) % mod;
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7378230014817902::solve();
    return 0;
}
