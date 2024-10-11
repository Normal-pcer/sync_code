#include <cstdio>
#include <cstring>
const int inf = 0xf0f0f0f0;
int F[2][1<<10], cnt[1<<10];
#define max(a,b) ((a)>(b)?(a):(b))
int main()
{
    freopen("N2171.in", "r", stdin);
    freopen("N2171.out", "w", stdout);
    int N, K, A, B, mask, i, j, h;
    scanf("%d%d%d%d", &N, &K, &A, &B);
    mask = (1<<K)-1;
    h = 1<<(K-1);
    for(i=1; i<=mask; ++i)
        cnt[i] = cnt[i>>1]+(i&1);
    int tot = 0, x;
    for(i=1; i<=N; ++i)
    {
        scanf("%d", &x); tot += x;
        memset(F[i&1], inf, sizeof(F[0]));
        for(j=0; j<=mask; ++j)
            if(A<=cnt[j] && cnt[j]<=B)
                F[i&1][j] = max(F[(i-1)&1][j>>1], F[(i-1)&1][(j>>1)|h]) + ((j&1)?x:0);
    }
    int ans = inf;
    for(i=0; i<=mask; ++i)
        ans = max(ans, F[N&1][i]);
    printf("%d", ans+ans-tot);
}