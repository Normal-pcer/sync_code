#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int INF = 0x3f3f3f3f;
const LL mod = 1e9 + 7;
const int N = 1000005;
LL c1[N], c2[N];
LL sum(int x) {
    LL res = 0;
    for (int i = x; i > 0; i -= i & -i) {
        res += c1[i] * (x + 1) - c2[i];
    }
    return res;
}
void add(int x, int d, int n) {
    for (int i = x; i <= n; i += i & -i) {
        c1[i] += d;
        c2[i] += (LL)d * x;
    }
}
int a[N], b[N], d[N], last[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i - 1] = a[i];
    }
    sort(b, b + n);
    int m = unique(b, b + n) - b;
    for (int i = 1; i <= n; i++) {
        a[i] = lower_bound(b, b + m, a[i]) - b;
        last[i] = d[a[i]];
        d[a[i]] = i;
    }
    LL ans = 0, now = 0;
    for (int i = 1; i <= n; i++) {
        now += i - last[i] + 2 * (sum(i) - sum(last[i]));
        now %= mod;
        ans += now;
        add(last[i] + 1, 1, n);
        add(i + 1, -1, n);
    }
    printf("%lld\n", ans % mod);
    return 0;
}