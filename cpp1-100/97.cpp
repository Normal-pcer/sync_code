#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

const int maxN = 2005;
char S[maxN] = {' '};
char T[maxN] = {' '};
int N, M;

long long dp[maxN][maxN];     // S的前i项 完全匹配T的前j项的 子序列数量

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);

    scanf("%d%d", &N, &M);
    scanf("%s", S+1);
    scanf("%s", T+1);

    upto(i, M) 
        switch (T[i]) {
        case 'A':  T[i]='T'; break;
        case 'C':  T[i]='G'; break;
        case 'G':  T[i]='C'; break;
        case 'T':  T[i]='A'; break;
        }

    if (strlen(S) < strlen(T)) {
        std::swap(S, T);    // 确保S不短于T
        std::swap(N, M);
    }
    log("%s\n%s\n", S, T)
    // 尝试在S中匹配T（找出和T相同的子序列）
    upto(i, N) {
        upto(j, M) {
            dp[i][j] = dp[i-1][j];
            if (S[i] == T[j])  {
                if (j==1)   dp[i][j] += 1;
                else        dp[i][j] += dp[i-1][j-1];
            }
            log("%3lld", dp[i][j]);
        }
        log("\n");
    }
    printf("%lld\n", dp[N][M]);
    return 0;
}