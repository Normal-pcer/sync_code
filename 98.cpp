#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

struct Corner {int value, height, width;};

int N, M;
const int maxN = 302;
int values[maxN][maxN];
int sigma[maxN][maxN];  // 从(1, 1)到(i, j)矩形范围内的求和，即“前缀和”
Corner dp[maxN][maxN];     // (i, j)为右下角总数最大的合法矩形

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    upto(r, N)  upto(c, M)  scanf("%d", &values[r][c]);
    // 维护求和数组
    upto(r, N) { 
        upto(c, M) {
            sigma[r][c] = sigma[r-1][c] + sigma[r][c-1] + values[r][c] - sigma[r-1][c-1];
            log("%2d ", sigma[r][c]);
        }
        log("\n")
    }
    upto(r, N) {
        upto(c, M) {
            if (!values[r][c]) {
                log("(%2d, %1d, %1d)[%2d, %2d] ", dp[r][c].value, dp[r][c].height, dp[r][c].width, 0, 0);
                continue;
            }
            // 向右扩展一列
            int height = dp[r][c-1].height;
            int width = dp[r][c-1].width + 1;
            int dest = r - dp[r][c-1].height;
            int realTop = dest;  // 实际可以达到的最高点（不包含）
            for (int i=r; i>dest; i--) {
                if (values[i][c] == 0) {  // 这个点不可用
                    realTop = i;  // 记录为下方一个点
                    break;
                }
            }
            height = r - realTop;
            // 计算新的最高矩形的总和
            int newSum1 = sigma[r][c] - sigma[r-height][c] - sigma[r][c-width] + sigma[r-height][c-width];
            // 或者可以选择只包含单列
            realTop = 0;
            for (int i=r; i>0; i--) {
                if (values[i][c] == 0) {  // 这个点不可用
                    realTop = i;  // 记录为下方一个点
                    break;
                }
            }
            int height2 = r - realTop;
            int newSum2 = sigma[r][c] - sigma[r-height2][c] - sigma[r][c-1] + sigma[r-height2][c-1];
            if (newSum1 > newSum2)
                dp[r][c] = {newSum1, height, width};
            else 
                dp[r][c] = {newSum2, height2, 1};
            log("(%2d, %1d, %1d)[%2d, %2d] ", dp[r][c].value, dp[r][c].height, dp[r][c].width, newSum1, newSum2);
        }
        log("\n")
    }
}

/*
3 4
1 2 3 4
5 0 6 3
10 3 4 0
*/