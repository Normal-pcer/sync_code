/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_d
 */
#pragma GCC optimize(3)
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

/**
 * 感觉暴力做就行。
 */
namespace Solution_1455683887377590 {
    // 拆分小于 N 的整数为 cnt 段
    std::vector<int> res;
    auto split(int N, int cnt, auto &&func) -> void {
        debug  std::cout << "split " << N << " " << cnt << " " << res.size() << std::endl;
        if (static_cast<int>(res.size()) == cnt) {
            func(res);
            return;
        }

        for (auto i = 1; i <= N; i++) {
            res.push_back(i);
            debug  std::cout << "push" << i << ", size = " << res.size() << std::endl;
            split(N-i, cnt, func);
            debug  std::cout << "pop" << res.back() << std::endl;
            res.pop_back();
        }
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<std::string> parts(N);
        for (auto &x: parts)  std::cin >> x;
        std::vector<std::string> exists(M);
        for (auto &x: exists)  std::cin >> x;
        
        std::set<std::string> exists_s(exists.begin(), exists.end());
        ranges::sort(parts);
        auto letters = 0;
        for (auto const &x: parts)  letters += x.size();
        try {
            do {
                // 对于每一个单词排列，考虑下划线数量
                auto seg_count = static_cast<int>(parts.size()) - 1;
                auto underline_count = 16 - letters;
    
                split(underline_count, seg_count, [&](std::vector<int> const &cnts) -> void {
                    debug {
                        for (auto x: cnts)  std::cout << x << " ";
                        std::cout << std::endl;
                    }
                    auto sum = std::accumulate(cnts.begin(), cnts.end(), 0) + letters;
                    if (sum < 3)  return;
                    assert(sum <= 16);
                    std::string s = parts[0];
                    for (auto i = 1; i < static_cast<int>(parts.size()); i++) {
                        assert(cnts[i-1] >= 1);
                        for (auto j = 0; j < cnts[i-1]; j++)  s += "_";
                        s += parts[i];
                    }
                    assert(3ULL <= s.size() and s.size() <= 16ULL);
                    debug  std::cout << s << std::endl;
                    if (not exists_s.contains(s)) {
                        std::cout << s << std::endl;
                        throw 0;
                    }
                });
            } while (std::next_permutation(parts.begin(), parts.end()));
        } catch (int x) {
            assert(x == 0);
            return;
        }
        std::cout << -1 << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1455683887377590::solve();
    return 0;
}
