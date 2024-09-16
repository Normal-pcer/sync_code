#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

struct Book { int height; int width; };
int dp[101][101];  // 第i本之前，共去掉j本; “不整齐度”的变化量
int N, M;
Book books[101];

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    upto(i, N) {
        int h, w;
        scanf("%d%d", &h, &w);
        books[i] = {h, w};
    }
    std::sort(books+1, books+1+N, [](Book a, Book b){ return a.height<b.height; });
    int ans=0;
    for (int i=2; i<=N; i++) {
        ans += abs(books[i].width - books[i-1].width);
    }
    upto(j, M) {
        upto(i, N) {
            dp[i][j] = std::min( 
                dp[i-1][j],  // 不动
                dp[i-1][j-1] - abs(books[i].width - books[i-1].width) - abs(books[i].width - books[i+1].width) + abs(books[i+1].width - books[i-1].width)  // 动
            );
            log("%4d(%2d, %2d)", dp[i][j], dp[i-1][j], dp[i-1][j-1] - abs(books[i].width - books[i-1].width) - abs(books[i].width - books[i+1].width) + abs(books[i+1].width - books[i-1].width));
        }
        log("\n");
    }
    printf("%d\n", ans+dp[N][M]);
    return 0;
}