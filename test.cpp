#include <iostream>
#include <vector>
#define endl "\n"
using namespace std;

const int MOD = 1000000007;

// 快速幂函数，用于计算 (base^exp) % MOD
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// 预计算阶乘和逆元阶乘
long long max_nm = 2e6;
vector<long long> fact(max_nm + 1, 1);
vector<long long> inv_fact(max_nm + 1, 1);
void init() {
    for (long long i = 1; i <= max_nm; ++i) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    
    inv_fact[max_nm] = mod_pow(fact[max_nm], MOD - 2, MOD);
    for (long long i = max_nm - 1; i >= 0; --i) {
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % MOD;
    }
}

void main_function() {
    long long n, m;
    cin >> n >> m;

    if (n == 0 && m == 0) {
        cout << 0 << endl;
        return;
    }
    

    // 计算组合数 C(n+m, n)
    long long a = n + m;
    long long b = n;
    long long comb = 0;
    if (a >= 0 && b >= 0 && b <= a) {
        comb = (fact[a] * inv_fact[b]) % MOD;
        comb = (comb * inv_fact[a - b]) % MOD;
    }

    long long denominator = (n + m) % MOD;
    if (denominator == 0) {
        cout << 0 << endl;
        return;
    }

    long long numerator = (denominator + 2 * m * n) % MOD;
    long long term = (numerator * mod_pow(denominator, MOD - 2, MOD)) % MOD;
    long long ans = (comb * term) % MOD;

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);
    init();
    int T;
    cin >> T;
    while (T--) {
        main_function();
    }
    return 0;
}