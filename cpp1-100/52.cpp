#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define ROWS_MAX 12
#define COLS_MAX 12

char map[ROWS_MAX][COLS_MAX];
bool walked[ROWS_MAX][COLS_MAX];
int N /* rows */, M /* columns */;
const int dx[] = { 0,  0,  0, +1, -1};
const int dy[] = { 0, +1, -1,  0,  0};

struct Position { int r, c, steps, health; };


int main() {
    scanf("%d %d", &N, &M);
    upto(r, N) {
        upto(c, M) {
            int temp;                       scanf("%d", &temp);
            map[r][c] = temp+'0';
        }
    }

    Position outset;
    upto(r, N)  upto(c, M)  if (map[r][c] == '2') { outset = {r, c, 0, 6}; }

    std::queue<Position> Q;         Q.push(outset);
    while ( Q.size() ) {
        auto now=Q.front();             Q.pop();
        debug printf("(%2d, %2d) %4d %4d %c\n", now.r, now.c, now.health, now.steps, map[now.r][now.c]);
        if (walked[now.r][now.c])       continue;
        if (now.health <= 0)            continue;
        if (map[now.r][now.c] != '4')   walked[now.r][now.c] = true;
        else                            now.health = 6;
        if (map[now.r][now.c] == '3') {
            printf("%d\n", now.steps);      return 0;
        }
        upto(i, 4) {
            Position t = {now.r+dx[i], now.c+dy[i], now.steps+1, now.health-1};
            if (map[t.r][t.c] == '0' || map[t.r][t.c] == 0)
                continue;
            if (0<=t.r && t.r<=N && 0<=t.c && t.c<=M)
                Q.push(t);
        }
    }
    printf("-1\n");
    return 0;
}

/*
3 3
2 1 1
1 1 0
1 1 3
*/