#include <bits/stdc++.h>

#if true
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif

#define filename "control"
#define debug if (debug_mode)
const bool debug_mode = false;
using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;
const long long infLL = 0x3f3f3f3f3f3f3f3fLL;

template <typename T>  inline T &chkMin(T &base, const T &cmp) {  return (base = std::min(base, cmp));  }
template <typename T>  inline T &chkMax(T &base, const T &cmp) {  return (base = std::max(base, cmp));  }

#define at operator[]
namespace Solution {
    // 三进制形式输出 x，保留 K 位
    void print(int x, const int K) {
        for (auto i = 0; i < K; i++) {
            std::cout << static_cast<char>('A' + (x % 3));
            x /= 3;
        }
    }

    inline std::string sprint(int x, const int K) {
        std::string ans;
        ans.resize(K);
        for (auto i = 0; i < K; i++) {
            ans[i] = static_cast<char>('A' + (x % 3));
            x /= 3;
        }
        return ans;
    }

    const constexpr int _pow3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969};
    constexpr inline int pow3(int x) {  return _pow3[x]; }

    // 获取三进制位（个位对应 pos = 0）
    constexpr inline int getDigit(int x, int pos) {
        return x / pow3(pos) % 3;
    }

    // 设置三进制位（个位 pos = 0）
    constexpr inline int setDigit(int x, int pos, int to) {
        assert(0 <= to and to <= 2);
        // debug  std::cout << "setDigit ", print(x, 3), std::cout << " at pos " << pos << " to " << to << " -> ", print(x - getDigit(x, pos) + to * pow3(pos), 3), std::cout << std::endl;
        return x + (to - getDigit(x, pos)) * pow3(pos);
    }

    constexpr inline int nearMod(int x, const int mod) {
        int res = x;
        if (res < 0)  res += mod;
        if (res >= mod)  res -= mod;
        // assert(0 <= res and res < mod);
        return res;
    }

    constexpr inline int transformDigit(int x, char op) {
        // assert(0 <= x and x <= 2);
        if (op == '+')  return nearMod(x+1, 3);
        if (op == '-')  return nearMod(x-1, 3);
        return x;
    }

    constexpr int reverseNumber(int origin) {
        int res = 0;
        while (origin) {
            res *= 3, res += (origin % 3);
            origin /= 3;
        }
        return res;
    }

    int transform(int origin, std::string const &program) {
        int result = origin;
        for (auto i = 0; i < (int)program.size(); i++) {
            auto cur = getDigit(result, i);
            auto op = program[i];
            auto to = transformDigit(cur, op);
            result = setDigit(result, i, to);
        }
        
        return result;
    }

    const int _K = 10;
    // std::array<std::array<ull, pow3(_K)>, 2> F;  // 经过 i 次操作使得状态为 j
    ull F[2][pow3(_K)];
    bool flag[pow3(_K)];

    void solve() {
        int N, K, MOD;  std::cin >> N >> K >> MOD;

        std::vector<std::string> programs(N);
        for (auto &s: programs)  std::cin >> s;

        // 三进制状压
        // 0-绿 1-黄 2-红
        // 考虑第 i 个程序，对于每一种状态，可以选择通过这种方式进行转移
        // 复杂度 O(N*K 3^K)
        
        F[0][0] = 1;
        const ull danger = 0x7000000000000000ULL;  // ull 要炸了
        flag[0] = true;
        for (auto c = 1; c <= N; c++) {
            int i = c & 1;
            std::memset(F[i], 0, sizeof(F[i]));
            for (auto j = 0; j < pow3(K); j++) {
                auto new_st = transform(j, programs[c-1]);
                F[i][j] += F[i^1][j];
                if (F[i][j] > danger)  F[i][j] %= MOD;
                F[i][new_st] += F[i^1][j];
                if (F[i][new_st] > danger)  F[i][new_st] %= MOD;
                if (flag[j])  flag[new_st] = true;
            }
        }

        std::vector<std::pair<std::string, int>> ans;
        ans.reserve(pow3(K));
        for (auto j = 0; j < pow3(K); j++) {
            if (flag[j]) {
                ans.push_back({sprint(j, K), F[N&1][j] % MOD});
            }
        }

        std::sort(ans.begin(), ans.end());

        for (auto [x, val]: ans) {
            std::cout << x << ' ' << val << std::endl;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(filename ".in", "r", stdin);
    std::freopen(filename ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    
    return 0;
}