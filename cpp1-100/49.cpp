#include <cstdio>
#include <queue>
#include <cstring>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG false
#define debug if(DEBUG)
#define ROWS_MAX 205
#define COLS_MAX 205

struct Position { int r, c, step; };
char map[ROWS_MAX][COLS_MAX];
bool visited[ROWS_MAX][COLS_MAX];
int N /*rows*/, M /*columns*/;

const int dx[] = { 0,  0,  0, +1, -1};
const int dy[] = { 0, -1, +1,  0,  0};

bool available(Position pos)  { return ( 1<=pos.r && pos.r<=N ) && (1<=pos.c && pos.c<=M); }

int main() {
    scanf("%d%d\n", &N, &M); 
    upto(r, N) {
        char line[COLS_MAX+1];
        scanf("%s", line);
        upto(c, M)  map[r][c] = line[c-1];
    }
    debug   upto(r, N)  printf("%s\n", map[r]);
    Position outset;
    upto(r, N) upto(c, M)  {
        if (map[r][c] == 'r') { outset = {r, c, 0}; }
    }
    std::queue<Position> Q;                 Q.push(outset);
    debug printf("(%d, %d)", outset.r, outset.c);
    while (Q.size()) {
        Position current = Q.front();           Q.pop();
        if (visited[current.r][current.c])      continue;
        if (map[current.r][current.c] == '#') {
            current.step++;                     Q.push(current);
            map[current.r][current.c] = '@';    continue;
        }
        if (map[current.r][current.c] == 'a') {
            printf("%d\n", current.step);       return 0;
        }
        visited[current.r][current.c] = true;
        debug printf("(%d, %d) %d\n", current.r, current.c, current.step);
        upto(i, 4) {
            Position target = {current.r+dx[i], current.c+dy[i], current.step+1};
            if (!available(target))                 continue;
            if (map[target.r][target.c] == '#')     continue;
            if (map[target.r][target.c] == 'x')     map[target.r][target.c] = '#';
            Q.push(target);
        }
    }
    printf("Impossible\n");
    return 0;
}

/*
7 8
#@#####@
#@a#@@r@
#@@#x@@@
@@#@@#@#
#@@@##@@
@#@@@@@@
@@@@@@@@
*/