#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 10001;
int b[maxN];
int dp[maxN];
long long count[maxN];
int N;
long long maxDp, maxCount;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    upto(i, N)  scanf("%d", b+i);
    std::fill(count+1, count+1+N, 1);
    upto(i, N) {
        for (int j=i-1; j>=1; j--) {
            if (b[j] <= b[i]) {
                if (dp[j]+1 > dp[i]) {
                    dp[i] = dp[j]+1;
                    count[i] = count[j];
                    // log("\n!!!%d %d(%d %d)\n", i, j, dp[i], dp[j]);
                } else if (dp[j]+1 == dp[i]) {
                    count[i]+=count[j];
                    // log("\n%d %d(%d %d)\n", i, j, dp[i], dp[j]);
                }
            }
        }
        if (dp[i] == maxDp) {
            maxCount += count[i];
        }
        if (dp[i] > maxDp) {
            maxDp = dp[i];
            maxCount = count[i];
        }
        log ("%d(%lld) ", dp[i], count[i]);
    }
    printf("%lld\n%lld\n", maxDp+1, maxCount);
    return 0;
}

/*
7
1 4 3 2 6 5 7
*/

/*
4
6
*/