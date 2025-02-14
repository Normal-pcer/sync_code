/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

/**
 * 选定两个字符串，除此以外的其他字符串都是固定的。
 * 其他字符串的贡献不会因为它们两个的顺序而改变。
 * 
 * 尝试交换两个字符串。
 * 记一个字符串内部的分数为 inner[i]，X 数量为 count[x]，数字总和为 num[x]。
 * 靠后的字符串 j 会产生 count[j] * num[i] 的贡献。
 * 如果交换字符串可以提升分数则交换，做一遍快排就行。
 */
namespace Solution_1687195712627777 {
    struct Info {
        ll inner;
        ll count, num;

        static auto of(std::string const &s) -> Info {
            auto x_count = 0;
            auto num_sum = 0;
            auto score = 0LL;

            for (auto ch: s) {
                if (ch == 'X')  x_count++;
                else {
                    assert('0' <= ch and ch <= '9');
                    score += (ch - '0') * x_count;
                    num_sum += (ch - '0');
                }
            }
            return {score, x_count, num_sum};
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::string> str(N);
        for (auto &x: str)  std::cin >> x;

        std::vector<Info> infos(N);
        ranges::transform(str, infos.begin(), Info::of);
        
        std::vector<int> iota(N);
        std::iota(iota.begin(), iota.end(), 0);
        auto cmp = [&](int x, int y) -> bool {
            auto info_x = infos[x];
            auto info_y = infos[y];
            return info_y.num * info_x.count > info_x.num * info_y.count;
        };
        std::sort(iota.begin(), iota.end(), cmp);

        std::string ans;
        for (auto x: iota)  ans += str[x];

        auto score = Info::of(ans).inner;
        std::cout << score << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1687195712627777::solve();
    return 0;
}
