#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);
    int n;
    double m;
    std::cin >> n >> m;

    int s = n * (n+1) / 2;
    double one = (double)100.0 / s;

    upto(i, n) {
        if (i * one >= m) {
            std::cout << i << std::endl;    return 0;
        } 
    }
    std::cout << -1 << std::endl;
    return 0;
}
