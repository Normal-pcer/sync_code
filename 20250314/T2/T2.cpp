#include <bits/stdc++.h>
#define FILENAME "T2"

using i32 = std::int32_t; using i64 = std::int64_t;
using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
char constexpr endl = '\n';

namespace IO
{
#define gc()(xS==xTT&&(xTT=(xS=xB)+fread(xB,1,1<<20,stdin),xS==xTT)?0:*xS++)
#define pc(x)(p3-obuf<1000000)?(*p3++=x):(fwrite(obuf,p3-obuf,1,stdout),p3=obuf,*p3++=x)
char xch,xB[1<<20],*xS=xB,*xTT=xB,obuf[1000000],*p3=obuf;
inline int read()
{
    char ch=gc();int x=0,f=1;
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=gc();}
    while('0'<=ch&&ch<='9'){x=(x<<1)+(x<<3)+(ch^48);ch=gc();}
    return x*f;
}
}
using IO::read;

i64 constexpr inf64 = 0x3f3f3f3f3f3f3f3fLL;

/**
 * F[i][j][k] 表示第 k 个人走到点 (i, j) 时的最小分数。
 * 从每个人的起点开始跑一遍最短路。
 * 中间判一下负环即可。
 * 
 * 看起来 124 子任务都能过，35 似乎会 TLE
 * 期望得分 35
 */
namespace Solution {
    auto solve() -> void {
        i32 N = read(), M = read(), Q = read();
        
        std::vector<std::vector<i32>> mat(N, std::vector<i32>(M));
        for (auto &line: mat) {
            for (auto &item: line)  item = read();
        } 

        auto spfa = [&](i32 x, i32 y, std::vector<std::vector<i64>> &dis) -> bool {
            for (auto &line: dis) {
                std::fill(line.begin(), line.end(), inf64);
            }

            std::vector<std::vector<char>> vis(N, std::vector<char>(M));
            std::vector<std::vector<i32>> cnt(N, std::vector<i32>(M));
            struct Point { i32 x, y; };
            std::deque<Point> q;
            dis[x][y] = mat[x][y], vis[x][y] = true;
            q.push_back({x, y});
            while (not q.empty()) {
                auto p = q.front();  q.pop_front();
                vis[p.x][p.y] = false;
                for (auto delta: {0x10, 0x12, 0x21, 0x01}) {
                    auto dx = (delta >> 4) - 1, dy = (delta & 15) - 1;
                    auto nx = p.x + dx, ny = p.y + dy;
                    if (nx < 0 or nx >= N)  continue;
                    if (ny < 0 or ny >= M)  continue;
                    if (dis[nx][ny] > dis[p.x][p.y] + mat[nx][ny]) {
                        dis[nx][ny] = dis[p.x][p.y] + mat[nx][ny];
                        cnt[nx][ny] = cnt[p.x][p.y] + 1;
                        if (cnt[nx][ny] >= N * M)  return false;
                        if (not vis[nx][ny])  q.push_back({nx, ny}), vis[nx][ny] = true;
                    }
                }
            }
            return true;
        };

        std::vector<std::vector<std::vector<i64>>> min_dis(Q);
        for (i32 q = 0; q < Q; q++) {
            i32 x, y;  x = read(), y = read();
            x--, y--;
            min_dis[q] = std::vector<std::vector<i64>>(N, std::vector<i64>(M));
            auto ok = spfa(x, y, min_dis[q]);
            if (not ok) {
                std::cout << "No" << endl;
                return;
            }
        }

        i64 ans = inf64;
        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j < M; j++) {
                i64 cur = -inf64;
                for (i32 q = 0; q < Q; q++) {
                    cur = std::max(cur, min_dis[q][i][j]);
                }
                ans = std::min(ans, cur);
            }
        }
        std::cout << ans << endl;
    }
}

int main() {
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    return 0;
}
