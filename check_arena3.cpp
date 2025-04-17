#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef pair<int, int> pii;

const int MAXN = 1e5 + 5;
int fa[MAXN];

int getfa(int x) {
    if (fa[x] == x) return x;
    return fa[x] = getfa(fa[x]);
}

void merge(int x, int y) {
    fa[getfa(x)] = getfa(y);
}

//以上两个并查集核心函数

set<int> s[MAXN];//set 保存每个岛屿/联盟所敌对的岛屿/联盟

int n, m, q;

int main() {
    cin >> n >> m >> q;
    for (int i = 0; i <= n; i++) {
        fa[i] = i;
    }//初始化并查集
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        s[u].insert(v);
        s[v].insert(u);//互相敌对
    }
    for (int i = 1; i <= q; i++) {
        int u, v;
        cin >> u >> v;
        int fau = getfa(u);
        int fav = getfa(v);
        if (s[fau].size() > s[fav].size()) swap(fau, fav);//启发式合并，小的合并入大的
        if (s[fau].find(fav) == s[fau].end()) {//如果找不到 father of u，即 u 与 v 不敌对，此时可合并
            printf("agree.\n");
            for (auto x : s[fau]) {//将小集合所有敌对边转移到代表上
                s[fav].insert(x);
                s[x].erase(fau);//删除原有边
                s[x].insert(fav);
            }
            merge(fau, fav);//并查集合并
        } else {
            printf("no!\n");
        }
    }
    return 0;
}