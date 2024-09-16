#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int main(int argc, char const *argv[]) {
    initDebug;
    int t, d, t1, d1, t2, d2;
    std::cin >> t >> d >> t1 >> d1 >> t2 >> d2;
    if (d1 > d2) {
        std::swap(d1, d2);
        std::swap(t1, t2);
    }   // d1 <= d2
    if (d > d2) {
        std::cout << -1 << std::endl;
    } else if (d > d1) {
        std::cout << std::max(0, t2-t) << std::endl;
    } else {
        std::cout << std::min( std::max(0, t2-t), std::max(0, t1-t) ) << std::endl;
    }
    return 0;
}
