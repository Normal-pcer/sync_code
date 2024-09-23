#include <cstdio>
#include <queue>
#include <cstring>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG false
#define debug if(DEBUG)
#define ROWS_MAX 205
#define COLS_MAX 205
#define T_MAX 11

struct Position { int r, c, step, health; };
char map[ROWS_MAX][COLS_MAX];
////bool visited[ROWS_MAX][COLS_MAX];
int minLength[ROWS_MAX][COLS_MAX][T_MAX];
int N /*rows*/, M /*columns*/, T /*Initial-health*/;

const int dx[] = { 0,  0,  0, +1, -1};
const int dy[] = { 0, -1, +1,  0,  0};

bool available(Position pos)  { return ( 1<=pos.r && pos.r<=N ) && (1<=pos.c && pos.c<=M); }

int main() {
    scanf("%d%d%d\n", &N, &M, &T); 
    upto(r, N) {
        char line[COLS_MAX+1];
        scanf("%s", line);
        upto(c, M)  map[r][c] = line[c-1];
    }
    Position outset;
    upto(r, N) upto(c, M)  {
        if (map[r][c] == '@') { outset = {r, c, 0, T}; }
    }
    std::queue<Position> Q;                 Q.push(outset);
    while (Q.size()) {
        Position current = Q.front();           Q.pop();
        if ( minLength[current.r][current.c][current.health] && current.step != 0 )    continue;
        if (map[current.r][current.c] == '+') {
            printf("%d\n", current.step);       return 0;
        }
        minLength[current.r][current.c][current.health] = true;
        debug printf("(%d, %d, %d) %d\n", current.r, current.c, current.health, current.step);
        upto(i, 4) {
            Position target = {current.r+dx[i], current.c+dy[i], current.step+1, current.health};
            if (!available(target))     continue;
            if (map[target.r][target.c] == '#') {
                if (target.health>=1) {
                    target.health--;
                }
                else    continue;
            }
            Q.push(target);
        }
    }
    printf("-1\n");
    return 0;
}
