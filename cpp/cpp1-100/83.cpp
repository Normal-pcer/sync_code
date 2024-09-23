#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 102;
int T[maxN];
int up[maxN];  // 以T[i]结尾的上升
int down[maxN];  // 以T[i]开头的下降
int N;


int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    upto(i, N)  scanf("%d", T+i);
    upto(i, N) {
        for (int j = i-1; j >= 1; j--) {
            if (T[j] < T[i])    up[i] = std::max(up[i], up[j] + 1);
        }
    }
    for (int i=N; i >= 1; i--) {
        for (int j=i; j<=N; j++) {
            if (T[j] < T[i])    down[i] = std::max(down[i], down[j] + 1);
        }
    }
    int ans = 0;
    debug {
        upto(i, N)  printf("%d ", up[i]);
        printf("\n");
        upto(i, N)  printf("%d ", down[i]);
        printf("\n");
    }
    upto(i, N)  ans = std::max(ans, up[i] + down[i]);
    log("%d %d\n", ans, 0);
    printf("%d\n", (N-ans-1));
    return 0;
}