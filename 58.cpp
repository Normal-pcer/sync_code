#include <bits/stdc++.h>
#define upto(i,e, n) for(int i=e;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)

int N, h[100005], dp[100005];
inline int get(int* arr, int index) {
    if (index<=0 || index>N)  return 0;
    return arr[index];
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin.tie(0);
    std::cin >> N;
    upto(i,1, N) {
        std::cin >> h[i];
    }
    upto(i,2, N) {
        dp[i] = std::min( std::abs(get(h, i-1)-get(h, i)) + get(dp, i-1), std::abs(get(h, i-2)-get(h, i)) + get(dp, i-2) );
        debug printf("%d(%d, %d) ", dp[i], std::abs(get(h, i-1)-get(h, i)) + get(dp, i-1), std::abs(get(h, i-2)-get(h, i)) + get(dp, i-2));
    }
    std::cout << dp[N] << std::endl;
    return 0;
}