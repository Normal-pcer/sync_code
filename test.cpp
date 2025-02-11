#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 5e5 + 5;

int val[N], dep[N], fa[N][20];
LL sum[N], f[N];
vector<int> G[N];

void dfs(int u, int father) {
    dep[u] = dep[father] + 1;
    fa[u][0] = father;
    for (int i = 1; i < 20; ++i)
        fa[u][i] = fa[fa[u][i-1]][i-1];
    
    sum[u] = val[u];
    LL sum_vd = 0;
    for (int v : G[u]) {
        if (v == father) continue;
        dfs(v, u);
        sum[u] += sum[v];
        sum_vd += f[v] + sum[v] * dep[v];
    }
    f[u] = sum_vd - sum[u] * dep[u];
}

int findZ(int x, int y) {
    if (dep[x] <= dep[y] + 1) return -1;
    for (int i = 19; i >= 0; --i) {
        if (dep[fa[x][i]] > dep[y] + 1)
            x = fa[x][i];
    }
    return fa[x][0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> val[i];
    for (int i = 2; i <= n; ++i) {
        int p; cin >> p;
        G[p].push_back(i);
    }
    dfs(1, 0);
    while (q--) {
        int x, y;
        cin >> x >> y;
        if (fa[x][0] == y) {
            cout << f[y] << '\n';
            continue;
        }
        int z = findZ(x, y);
        LL delta = (sum[z] - sum[x]) + val[x] * (1 - (dep[x] - dep[y]));
        cout << f[y] + delta << '\n';
    }
    return 0;
}