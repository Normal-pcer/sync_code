#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int N;
char text[1002]=" ";
char text_reversed[1002]=" ";
int dp[1002][1002];         // t[:i+1], tr[:i+1]最长公共子序列的长度

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    scanf("%d", &N);
    upto(_, N) {
        scanf("%s", &text[1]);
        int length = (int)strlen(text);
        // std::reverse_copy(text+1, text+strlen(text), text_reversed+1);
        // std::reverse_copy(text.begin(), text.end(), text_reversed.begin());
        upto(i, length)     text_reversed[length-i] = text[i];
        std::memset(dp, 0, sizeof(dp));
        int ans = 0;
        upto(i, length-1) {
            upto(j, length-1) {
                dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                if (text[i] == text_reversed[j])
                    dp[i][j] = std::max(dp[i][j], dp[i-1][j-1]+1);
                ans = std::max(ans, dp[i][j]);
                log("%3d", dp[i][j])
            }
            log("\n")
        }
        printf("%d\n", length-ans-1);
    }
}