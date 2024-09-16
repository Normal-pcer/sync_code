#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define L std::

int dp[21]={0, 0, 0, 2};
int main() {
    int n;
    L cin >> n;
    for (int i=4; i<=n; i++)    dp[i] = (i-3)*dp[i-1];
    L cout << dp[n] << L endl;
    return 0;
}