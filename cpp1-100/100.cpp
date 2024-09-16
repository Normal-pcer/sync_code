#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

int N, M;
const int maxN = 550;
const int maxM = (int)3e5+5;
std::vector< std::vector<short> > values;
// int sigma[maxN][maxM];      // (1,1)到(i,j)矩形范围求和
std::vector< std::vector<int> > sigma;
// int columns[maxN];
int dp[maxM];
int ans;
std::vector<int> line;
std::vector<short> sline;


int sumRect(int T, int L, int B, int R) {
    return sigma[B][R] - sigma[B][L-1] - sigma[T-1][R] + sigma[T-1][L-1];
}

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    // 保证N<=M
    short temp;
    if (N>=M) {
        std::swap(N, M);
        sline.reserve(M+1);
        upto(c, M+1)    sline.push_back(0);
        values.push_back(sline);
        upto(c, M) {
            sline.clear();
            sline.push_back(0);
            upto(r, N) {
                sline.push_back(0);
            }
            values.push_back(sline);
        }
        upto(c, M)  upto(r, N)  scanf("%hd", &values[r][c]);
    } else {
        sline.reserve(M);
        upto(c, M+1)    sline.push_back(0);
        values.push_back(sline);
        upto(r, N) {
            sline.clear();
            sline.push_back(0);
            upto(c, M) {
                scanf("%hd", &temp);
                sline.push_back(temp);
            }
            values.push_back(sline);
        }
    }
    debug {
        upto(r, N) {
            upto(c, M)  printf("%4d", values[r][c]);
            printf("\n");
        }
    }
    // 维护求和数组
    line.reserve(M+1);
    upto(c, M+1)    line.push_back(0);
    sigma.push_back(line);
    upto(r, N) {
        line.clear();
        line.push_back(0);
        upto(c, M) {
            line.push_back(sigma[r-1][c] + line[c-1] + values[r][c] - sigma[r-1][c-1]);
            log("%2d ", line[c]);
        }
        sigma.push_back(line);
        upto(c, M) {
            log("!%2d ", sigma[r][c]);
        }
        log("\n")
    }
    upto(top, N) {                                      // 上边线
        log("\n\n\nt=%d", top)
        for (int bottom=N; bottom>=top; bottom--) {     // 下边线
            log("\nb=%d\n", bottom)
            upto(i, M) {
                dp[i] = std::max(dp[i-1], 0) + sumRect(top, i, bottom, i);
                ans = std::max(ans, dp[i]);
                log("%5d(%d)", dp[i], sumRect(top, i, bottom, i));
            }
        }
    }
    log("\n")
    printf("%d\n", ans);
    return 0;
}

/*
3 4
1 2 3 4
5 -20 6 3
10 3 4 -10
*/

/*
4 3
4 3 -10
3 6 4
2 -20 3
1 5 10 
*/