#include <bits/stdc++.h>

#define FILE_NAME "edit"
#define FILE_IO_ENABLED false

using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;

template <typename T>  T &chkMax(T &base, const T &cmp) { return (base = std::max(base, cmp)); }
template <typename T>  T &chkMin(T &base, const T &cmp) { return (base = std::min(base, cmp)); }

namespace Solution {

    const int _N = 100'005;
    int F[_N][2][2];

    void solve() {
        std::memset(F, -0x3f, sizeof(F));
        
        int N;  std::cin >> N;
        std::vector<std::string> _s(2), _mask(2);  // Storage from index 1
        std::cin >> _s[0] >> _s[1] >> _mask[0] >> _mask[1];
        _s[0] = '0' + _s[0], _s[1] = '1' + _s[1];
        _mask[0] = '0' + _mask[0], _mask[1] = '0' + _mask[1];

        std::vector<std::vector<char>> s(2, std::vector<char>(N+1)), 
                    mask(2, std::vector<char>(N+1));
        
        std::transform(_s[0].begin(), _s[0].end(), s[0].begin(), [](char x) -> char { return x == '1'; });
        std::transform(_s[1].begin(), _s[1].end(), s[1].begin(), [](char x) -> char { return x == '1'; });
        std::transform(_mask[0].begin(), _mask[0].end(), mask[0].begin(), [](char x) -> char { return x == '1'; });
        std::transform(_mask[1].begin(), _mask[1].end(), mask[1].begin(), [](char x) -> char { return x == '1'; });

        if (_mask.at(0) == _mask.at(1)) {
            s.at(0).push_back(0), s.at(1).push_back(1);
            mask.at(0).push_back(0), mask.at(1).push_back(0);
            std::array<std::array<int, 2>, 2> count;
            auto ans = 0;
            count.at(0).at(0) = count.at(0).at(1) = count.at(1).at(0) = count.at(1).at(1) = 0;
            
            for (auto i = 1; i <= N+1; i++) {
                if (not mask.at(0).at(i)) {
                    ans += std::min(count.at(0).at(0), count.at(1).at(0));
                    ans += std::min(count.at(0).at(1), count.at(1).at(1));
                    if (s.at(0).at(i) == s.at(1).at(i)) {
                        ans++;
                    }
                    count.at(0).at(0) = count.at(0).at(1) = count.at(1).at(0) = count.at(1).at(1) = 0;
                } else {
                    count.at(0).at(s.at(0).at(i))++, count.at(1).at(s.at(1).at(i))++;
                }

            }
            std::cout << ans << std::endl;
            return;
        }

        F[0][0][0] = 0;
        for (auto i = 0; i < N; i++) {
            for (auto j: {0, 1}) {
                for (auto k: {0, 1}) {
                    // Do not swap anything
                    chkMax(F[i+1][s[0][i+1]][s[1][i+1]], F[i][j][k] + (s[0][i+1] == s[1][i+1]));

                    bool flag0 = mask[0][i] and mask[0][i+1];
                    bool flag1 = mask[1][i] and mask[1][i+1];

                    // Swap s[0][i] with s[0][i+1] only
                    if (flag0) {
                        auto ans = F[i][j][k];
                        if (j == k)  ans--;
                        if (s[0][i+1] == k)  ans++;
                        if (j == s[1][i+1])  ans++;
                        chkMax(F[i+1][j][s[1][i+1]], ans);

                        // printf("F[%d][%d][%d] <= F[%d][%d][%d]\n", i+1, j, s[i][i+1], i, j, k);
                    }

                    // Swap s[1][i] with s[1][i+1] only
                    if (flag1) {
                        auto ans = F[i][j][k];
                        if (j == k)  ans--;
                        if (j == s[1][i+1])  ans++;
                        if (s[0][i+1] == k)  ans++;
                        chkMax(F[i+1][s[0][i+1]][k], ans);
                        // printf("F[%d][%d][%d] <= F[%d][%d][%d]\n", i+1, s[0][i+1], k, i, j, k);
                    }

                    // Swap both
                    if (flag0 and flag1) {
                        auto ans = F[i][j][k];
                        if (s[0][i+1] == s[1][i+1])  ans++;
                        chkMax(F[i+1][j][k], ans);
                        // printf("F[%d][%d][%d] <= F[%d][%d][%d]\n", i+1, j, k, i, j, k);
                    }

                    // std::cout << "F[" << i << "][" << j << "][" << k << "] = " << F[i][j][k] << std::endl;
                }
            }
        }

        auto ans = -1;
        for (auto j: {0, 1}) {
            for (auto k: {0, 1}) {
                chkMax(ans, F[N][j][k]);
            }
        }

        std::cout << ans << std::endl;
    }
}

int main() {
    // freopen
#if FILE_IO_ENABLED
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;  std::cin >> T;

    while (T --> 0)  Solution::solve();

    return 0;
}