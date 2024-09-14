#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int digitSum(int number) {
    int sum=0;
    while (number) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

int n, p;
char ans[1000005] = {'1'};
int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);
    std::cin >> n >> p;
    int imx = 9*n;
    int minMod = 0x7fffffff;
    for (int i=1; i<=imx; i++) {
        minMod = std::min(minMod, i%p);
    }
    std::fill(ans+1, ans+n+1, '0');
    minMod = (minMod-1+p)%p;
    debug printf("%d\n", minMod);
    int nineCount = minMod / 9;
    int remain = minMod % 9;
    log("%d %d\n", nineCount, remain)
    ans[n] = '\0';
    for (int i=n-1; i>=n-nineCount; i--)    ans[i] = '9';
    ans[n-nineCount-1] = '0' + remain;
    if (n-nineCount-1 == 0) ans[0]++;
    debug for (int i=0; i<=n; i++) putchar(ans[i]);
    log("\n")
    printf("%s\n", ans);
    return 0;
}
