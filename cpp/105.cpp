#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

int split[2005][105];
int N, a[105];
int sum;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    upto(i, N)  scanf("%d", a+i);
    upto(i, N)  sum += i;
    upto(i, N) {
        split[a[i]][i] = 1;
        upto(j, sum) {
            split[j][i] += split[j][i-1];
            // log("[%d][%d]=%d+[%d][%d]\n", j+a[i], i, split[j+a[i]][i], j, i-1)
            if (j>a[i]) {
                split[j][i] += split[j-a[i]][i-1];
                // if (split[j][i] >= 2)
            }
        }
    }
    debug {
        upto(i, N) {
            upto(j, sum) {
                log("%3d", split[j][i]);
            }
            log("\n")
        }
    }
    for (int i=sum; i>=1; i--) {
        if (split[i][N] >= 2) {
            printf("%d\n", i);
            return 0;
        }
    }
    printf("Impossible\n");
    return 0;
}