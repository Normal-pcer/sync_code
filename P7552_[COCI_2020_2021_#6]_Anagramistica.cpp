/**
 * @link https://www.luogu.com.cn/problem/P7552
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 两个单词相似，即二者所有字母组成的无序可重集合相等。
 * 希望选出任意多个单词，有 k 对相似的单词。
 * 对于一个字母集合，如果它出现了 N 次，则有 N * (N - 1) / 2 对单词相似。
 * 按照集合分组。
 * 设 F[i][j] 表示：前 i 个集合中任意选择，出现恰好 k 对相似的方案数。
 * 考虑转移：只需从第 i 个集合中选任意多个，从前 i - 1 个选择补齐即可。
 * F[i][j] = F[i-1][j-k] * ...
 * 
 */
namespace Solution_8467128197774301 {
    constexpr const int mod = 1e9+7;
    auto hashSet(std::string const &s) -> ull {
        static auto letter_val = []() {
            std::mt19937 rnd{745184};
            std::array<unsigned int, 26> res;
            std::generate(res.begin(), res.end(), rnd);
            return res;
        }();

        auto res = 0ULL;
        for (auto ch: s)  res += letter_val.at(ch - 'a');
        return res; 
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector<std::string> words(N);
        std::map<ull, int> groups;  // 每一组的单词数量
        for (auto &word: words) {
            std::cin >> word;
            groups[hashSet(word)]++;
        }

        std::vector<std::vector<int>> C(N+1);
        for (auto i: range(1, N+1)) {
            C[i].resize(i + 1);
            C[i][0] = C[i][i] = 1;
            for (auto j: range(1, i)) {
                C[i][j] = (C[i-1][j-1] + C[i-1][j]) % mod;
            }
        }

        std::vector F(groups.size()+1, std::vector<int>(K+1));
        F[0][0] = 1;
        for (auto i = 1; auto [set, cnt]: groups) {
            for (auto j: range(0, K+1)) {
                for (auto x: range(0, cnt+1)) {
                    auto k = x * (x - 1) / 2;
                    if (k > j)  break;
                    debug std::cout << std::format("F[{}][{}] <= F[{}][{}] * C[{}][{}]", i, j, i-1, j-k, cnt, k) << std::endl;
                    F[i][j] += static_cast<ll>(F[i-1][j-k]) * C[cnt][x] % mod;
                    F[i][j] %= mod;
                }
            }
            i += 1;
        }
        auto ans = F[groups.size()][K];
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8467128197774301::solve();
    return 0;
}
