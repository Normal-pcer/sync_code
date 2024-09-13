#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 100002;
int N, a[maxN];
int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> N;
    upto(i, N)  std::cin >> a[i];
    int sum = 0;
    int result = -2147483647;
    upto(i, N) {
        sum = std::max(0, sum) + a[i];
        result = std::max(result, sum);
    }
    std::cout << result << std::endl;
    return 0;
}