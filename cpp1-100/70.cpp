#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int MAX_SIZE = 1002;
int N, M;
char map[MAX_SIZE][MAX_SIZE];
int dp[MAX_SIZE][MAX_SIZE];
int ans = 0;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    upto(r, N)  upto(c, M)  scanf("%c ", &map[r][c]);
    upto(r, N)  upto(c, M)  {
        if (map[r][c] != '1')   continue;
        dp[r][c] = std::min({dp[r][c-1], dp[r-1][c-1], dp[r-1][c]}) + 1;
        ans = std::max(ans, dp[r][c]);
    }
    debug {
        upto(r, N) {
            upto(c, M) {
                printf("%4d", dp[r][c]);
            }
            printf("\n");
        }
    }
    printf("%d\n", ans);
    return 0;
}

/*
4 4
0 1 1 1
1 1 1 0
0 1 1 0
1 1 0 1
*/