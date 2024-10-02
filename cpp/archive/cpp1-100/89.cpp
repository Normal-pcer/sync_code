#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int x, y, w, k;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);
    std::cin >> x >> y >> w >> k;
    int growth = 0;
    int boost = 0;
    int i = 1;
    while (growth < k) {
        growth += int(i/w);
        if (i>=x) {
            boost += y;
            growth += boost;
        }
        i++;
    }
    std::cout << i-1 << std::endl;
    return 0;
}
