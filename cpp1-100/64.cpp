#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1) && !strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)

bool DEBUG_MODE;
const int MAX_SIZE = 100005;
int N;
int   A[MAX_SIZE],   B[MAX_SIZE],   C[MAX_SIZE];
int dp1[MAX_SIZE], dp2[MAX_SIZE], dp3[MAX_SIZE];
int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> N;
    upto(i, N) {
        std::cin >> A[i] >> B[i] >> C[i];
    }
    dp1[1] = A[1]; dp2[1] = B[1]; dp3[1] = C[1];
    for (int i=2; i<=N; i++) {
        dp1[i] = std::max(dp2[i-1], dp3[i-1]) + A[i];
        dp2[i] = std::max(dp3[i-1], dp1[i-1]) + B[i];
        dp3[i] = std::max(dp1[i-1], dp2[i-1]) + C[i];
        log("%d %d %d\n", dp1[i], dp2[i], dp3[i]);
    }
    std::cout << std::max( std::max(dp1[N], dp2[N]), dp3[N] ) << std::endl;
    return 0;
}