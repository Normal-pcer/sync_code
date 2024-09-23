#include <cstdio>
#define mod 2333333

int temp;
inline int getInt() {
    scanf("%d", &temp);
    return temp;
}

int dp[2][1005];

int main(int argc, char const* argv[]) {
    const int treeCount = getInt();
    const int outset = getInt();
    const int destination = getInt();
    const int timeLimit = getInt();

    dp[0][outset] = 1;

    for (int currentTime = 1; currentTime <= timeLimit; currentTime++) {
        for (int treeIndex = 1; treeIndex <= treeCount; treeIndex++) {
            dp[1][treeIndex] = (dp[0][treeIndex - 1] + dp[0][treeIndex + 1]) % mod;
        }
        for (int treeIndex = 1; treeIndex <= treeCount; treeIndex++) {
            dp[0][treeIndex] = dp[1][treeIndex];
        }
    }

    printf("%d", dp[1][destination]);
}