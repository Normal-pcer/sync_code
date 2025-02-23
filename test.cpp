#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 200005;

long long a[MAXN], sum[MAXN], inv[MAXN];
int freq[MAXN];

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

long long mod_inverse(long long x, long long mod) {
    return mod_exp(x, mod - 2, mod);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    // Reading the input array a[]
    for (int i = 1; i <= N; i++) {
        cin >> a[i];
    }

    // Precompute the modular inverses for each number from 1 to N
    inv[1] = 1;
    for (int i = 2; i <= N; i++) {
        inv[i] = mod_inverse(i, MOD);
    }

    // Initialize sum array and frequency counts
    memset(freq, 0, sizeof(freq));
    sum[0] = 0;
    for (int i = 1; i <= N; i++) {
        sum[i] = (sum[i-1] + a[i]) % MOD;
    }

    long long last_result = 0;

    // Process each value of K
    for (int K = 1; K <= N; K++) {
        long long val = a[K];
        
        // Calculate sum and frequency up to `val`
        long long part1 = (freq[val] * a[K]) % MOD;
        long long part2 = (sum[K - 1] - sum[val]) % MOD;

        part1 = (part1 + part2) % MOD;

        long long part3 = (2 * a[K]) % MOD;
        long long current_result = (last_result + (part1 + part3) % MOD) % MOD;

        last_result = current_result;

        // Multiply by modular inverse of x^2
        current_result = (current_result * inv[K]) % MOD;
        cout << current_result << endl;

        // Update the frequency for a[K]
        freq[a[K]]++;
    }

    return 0;
}
