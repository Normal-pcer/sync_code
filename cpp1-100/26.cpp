#include <bits/stdc++.h>
using namespace std;

long long dp[2][1005];
int N;

int main(int argc, char const* argv[]) {
    cin >> N;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= i; j++) {
            scanf("%d", &dp[1][j]);
            dp[1][j] += max(dp[0][j - 1], dp[0][j]);
        }
        for (int j = 1; j <= i; j++) {
            dp[0][j] = dp[1][j];
        }
    }
    int maxx = 0;
    for (int i : dp[1])  maxx = max(maxx, i);
    cout << maxx << endl;
    return 0;
}