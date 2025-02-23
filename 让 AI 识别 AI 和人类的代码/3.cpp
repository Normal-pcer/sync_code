// 3.cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200005;
const int MOD = 998244353;
inline long long fast_pow(long long x, long long y) {
    long long res = 1;
    while (y) {
        if (y & 1) res = res * x % MOD;
        x = x * x % MOD;
        y >>= 1;
    }
    return res;
}
struct FenwickTree {
    int n;
    vector<long long> tree;
    FenwickTree(int size) : n(size), tree(size + 1, 0) {}
    void update(int idx, long long val) {
        while (idx <= n) {
            tree[idx] = (tree[idx] + val) % MOD;
            idx += idx & -idx;
        }
    }
    long long query(int idx) const {
        long long res = 0;
        int i = idx;
        while (i > 0) {
            res = (res + tree[i]) % MOD;
            i -= i & -i;
        }
        return res;
    }
    long long query_range(int l, int r) const {
        if (l > r) return 0;
        return (query(r) - query(l - 1) + MOD) % MOD;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    vector<int> A(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> A[i];
    }
    
    FenwickTree sum_tree(MAXN), cnt_tree(MAXN);
    long long total_sum = 0;
    for (int i = 1; i <= N; ++i) {
        int val = A[i];
        
        long long cnt_le = cnt_tree.query(val);
        long long sum_le = sum_tree.query(val);
        
        long long cnt_gt = cnt_tree.query_range(val + 1, MAXN - 1);
        long long sum_gt = sum_tree.query_range(val + 1, MAXN - 1);
        
        total_sum = (total_sum + val + 2 * (sum_le + cnt_gt * val % MOD)) % MOD;
        
        long long inv_i = fast_pow(i, MOD - 2); 
        long long inv_i_sq = inv_i * inv_i % MOD; 
        cout << total_sum * inv_i_sq % MOD << "\n";
        
        sum_tree.update(val, val);
        cnt_tree.update(val, 1);
    }
    return 0;
}
