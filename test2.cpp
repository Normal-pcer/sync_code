#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;
  
const int maxN = 10001;
int b[maxN];
int dp[maxN];
int pos[maxN];
int ans[maxN];
int N;
  
int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    upto(i, N)  scanf("%d", b+i);
    upto(i, N) {
        for (int j=i-1; j>=1; j--) {
            if (b[j] <= b[i] && dp[j]+1 > dp[i]) {
                dp[i] = dp[j]+1;
                pos[i] = j;
            }
        }
    }
    int maxDp = -0x7fffffff;
    int maxIndex = -1;
    upto(i, N)  if (maxDp<dp[i]) {
        maxDp = dp[i];  maxIndex = i;
    }
    debug upto(i, N)  printf("%d ", dp[i]);
    log("\n");
    debug upto(i, N)  printf("%d ", pos[i]);
    log("\n");
    int pointer = maxIndex;
    int ansIndex = 1;
    pos[0] = -1;
    while (pos[pointer] >= 0) {
        ans[ansIndex++] = b[pointer];
        pointer = pos[pointer];
    }
    printf("%d", ans[--ansIndex]);
    while (--ansIndex)
        printf(" %d", ans[ansIndex]);
    printf("\n");
    return 0;
}