#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int n, m;
int friendly[102][102];

int getMax() {
    int m=-0x7fffffff;
    upto(i, n) {
        upto(j, n) {
            m = std::max(m, friendly[i][j]);
        }
    }
    return m;
}

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);
    std::cin >> n >> m;
    int op, a, b, c;
    upto(i, m) {
        std::cin >> op >> a >> b >> c;
        if (op == 1)    friendly[a][b] += c;
        else    friendly[a][b] -= c;
        std::cout << getMax() << std::endl;
    }
    return 0;
}
