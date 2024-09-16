#include <cstdio>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 1002;
int M, N;
int a[maxN], b[maxN];
int dp[maxN][maxN];     // a的前i项、b的前j项中最长公共子序列的长度
char dir[maxN][maxN];   // 方向
int ans[maxN];          // 用于最后反向回溯答案。答案的下标为1, 2, 3, ..., n

int main(int argc, char const *argv[]) {
    // initDebug;
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
            dir[i][j] = '^';    // 向上
            if (dp[i][j-1] > dp[i][j]) {
                dp[i][j] = dp[i][j-1];
                dir[i][j] = '<';    // 向左
            } else if (a[i] == b[j] && dp[i-1][j-1]+1 > dp[i][j]) {
                dp[i][j] = dp[i-1][j-1]+1;
                dir[i][j] = '@';    // 本身（向左上）
            }
            log("  %c", dir[i][j]);
        }
        log("\n");
    }

    int pointerR = M;   // 指针行号
    int pointerC = N;   // 指针列号
    int ansCount = dp[M][N];
    int ansIndex = ansCount;

    while (dp[pointerR][pointerC]) {
        char direction = dir[pointerR][pointerC];
        if (direction == '<')       pointerC--;
        else if (direction == '^')  pointerR--;
        else {
            ans[ansIndex--] = a[pointerR];
            pointerC--; pointerR--;
        }
    }

    while (++ansIndex <= ansCount) {
        printf("%d ", ans[ansIndex]);
    }
    printf("\n");
    return 0;
}