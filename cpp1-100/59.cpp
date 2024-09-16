#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);


int N, h[100005], dp[100005], K;
inline int get(int* arr, int index) {
    if (index<=0 || index>N)  return 0;
    return arr[index];
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin.tie(0);
    std::cin >> N >> K;
    upto(i, N) {
        std::cin >> h[i];
    }
    for (int i=2; i<=N; i++) {
        dp[i] = 2147483647;
        upto(j, K)  if (i-j>0)  dp[i] = std::min(dp[i], std::abs(get(h, i)-get(h, i-j)) + get(dp, i-j));
        log("%d ", dp[i]);
    }
    std::cout << dp[N] << std::endl;
    return 0;
}