#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 1005;
int M, N;
int a[maxN], b[maxN];
int dp[maxN][maxN];     // a的前i项、b的前j项中最长公共子序列的长度
int prev[maxN][maxN];   // 上述子序列最后一个字符在a中的下标
int p[maxN][maxN];      // 倒数第二个字符
int ans[maxN];          // 用于最后反向回溯答案。答案的下标为1, 2, 3, ..., n

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &M, &N);
    upto(i, M)  scanf("%d", a+i);
    upto(i, N)  scanf("%d", b+i);

    upto(i, M) {
        upto(j, N) {
            /*
            dp[i][j] = std::max({
                dp[i-1][j],                             // 忽略一个a中项
                dp[i][j-1],                             // 忽略一个b中项
                a[i]==b[j]? (dp[i-1][j-1]+1): 0         // 公共项
            });
            */
            dp[i][j] = dp[i-1][j];
            p[i][j] = prev[i][j];
            prev[i][j] = prev[i-1][j];
            if (dp[i][j-1] > dp[i][j]) {
                dp[i][j] = dp[i][j-1];
                p[i][j] = prev[i][j];
                prev[i][j] = prev[i][j-1];
                log(".");
            } else if (a[i] == b[j] && dp[i-1][j-1]+1 > dp[i][j]) {
                dp[i][j] = dp[i-1][j-1]+1;
                p[i][j] = prev[i][j];
                prev[i][j] = i;
                log(":");
            } else log("?");
            log("%d(%d|%d) ", dp[i][j], prev[i][j], p[i][j]);
        }
        log("\n");
    }

    // log("%d\n", prev[M][N]);
    int pointer = prev[M][N];
    int ansIndex = 0;
    while (pointer) {
        ans[++ansIndex] = a[pointer];
        pointer = p[pointer][N];
    }

    // debug upto(i, ansIndex)  printf("%d ", ans[i]);
    printf("%d", ans[ansIndex--]);
    for (int i=ansIndex; i>=1; i--)     printf(" %d", ans[i]);
    printf("\n");
    return 0;
}