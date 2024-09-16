#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);

int N, dp[10005], arr[10005];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin.tie(0);

    std::cin >> N;
    upto(i, N) {
        std::cin >> arr[i];
    }
    int ans=0;
    upto(i, N) {
        dp[i]=1;
        for (int j=i-1; j>=1; j--) {
            if (arr[i]>=arr[j])
                dp[i] = std::max(dp[i], dp[j]+1);
        }
            log("%d ", dp[i])
        ans=std::max(ans, dp[i]);
    }
    std::cout << ans << std::endl;
    return 0;
}