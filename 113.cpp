#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<=e;i++)
#define reverse(i,e,b) for(int i=e;i>=b;i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max(base,cmp)
#define chkMin(base,cmp) base=std::min(base,cmp)
#define update(base,op,modify) base=op(base,modify)
#define ensure(con,v) if(!(con))v=false;
bool DEBUG_MODE=false;


const int maxN = 103;
char a[maxN] = {};
char b[] = "0ABC";
int N;

int dp[maxN][5];  // a的前i项和"ABC"的前j项的最长公共子串

int main() {
    initDebug;
    scanf("%d", &N);
    scanf("%s", a+1);
    upto(i, N) {
        upto(j, 3) {
            dp[i][j] = a[i]==b[j]? (dp[i-1][j-1]+1): 0;
            log("%3d", dp[i][j])
            if (dp[i][j] == 3) {
                printf("%d\n", i-2);
                return 0;
            }
        }
        log("\n")
    }
    printf("%d\n", -1);
    return 0;
}