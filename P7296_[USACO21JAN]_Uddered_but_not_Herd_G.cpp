/**
 * @link https://www.luogu.com.cn/problem/P7296
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 考虑什么情况下一定会重新唱字母表？
 * 设 index[ch] 表示 ch 在字母表中的位置。index[ch] > index[ch + 1] 的时候一定是进行到下一次。
 * 统计两个字符成为按顺序相邻字符的次数。
 * 拿字母代替未知字母似乎有一些混乱。
 * 在一个字母表中，如果 甲 比 乙 先出现，那么每一对相邻都会产生对应的贡献。
 * 可能出现的字母有 20 个。枚举一个字母可能出现在谁之后，由于前面字母的先后顺序并不重要，这个过程可以状压。
 */
namespace Solution_2949460031004911 {
    auto solve_force() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::string s;  std::cin >> s;
        std::vector<char> letters;
        {
            letters.assign(s.begin(), s.end());
            ranges::sort(letters);
            auto tmp = ranges::unique(letters);
            letters.erase(tmp.begin(), tmp.end());
        }

        i32 ans = inf;
        do {
            i32 cur_ans = 1;
            assert(s.size() > 0);
            auto prev = s[0];
            for (auto ch: s | views::drop(1)) {
                auto cur = static_cast<uz>(ch);
                if (ranges::find(letters, cur) <= ranges::find(letters, prev))  cur_ans++;
                prev = cur;
            }
            chkMin(ans, cur_ans);
        } while (ranges::next_permutation(letters).found);

        std::cout << ans << endl;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::string s;  std::cin >> s;
        std::vector<char> letters;
        {
            letters.assign(s.begin(), s.end());
            ranges::sort(letters);
            auto tmp = ranges::unique(letters);
            letters.erase(tmp.begin(), tmp.end());
        }
        uz letters_count = letters.size();
        std::array<uz, 128> letters_unmap;
        letters_unmap.fill(-1);
        for (uz i = 0; i != letters_count; i++) {
            uz ch = letters[i];
            letters_unmap[ch] = i;
        }

        // ac[i][j] 表示有多少次出现连续的 i 和 j（按照顺序）
        std::vector adjacent_count(letters_count, std::vector<i32>(letters_count));
        assert(s.size() != 0);
        for (auto it = s.begin() + 1; it != s.end(); it++) {
            auto u = letters_unmap[static_cast<uz>(*std::prev(it))];
            auto v = letters_unmap[static_cast<uz>(*it)];
            adjacent_count[u][v]++;
        }

        using Status = u32;
        auto stat_count = (Status)1 << letters_count;

        std::vector<i32> F(stat_count, inf);

        auto j_end = (Status)1 << letters_count;
        F[0] = 1;
        for (Status j = 0; j != j_end; j++) {
            for (uz i = 0; i != letters_count; i++) {
                if (j & ((Status)1 << i))  continue;
                // 尝试把 i 加到 j 上
                i32 extra = 0;
                for (uz k = 0; k != letters_count; k++) {
                    if (j & ((Status)1 << k))  extra += adjacent_count[i][k];
                }
                extra += adjacent_count[i][i];
                chkMin(F[j | (Status)1 << i], F[j] + extra);
            }
        }

        auto ans = F.back();
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2949460031004911::solve();
    return 0;
}
