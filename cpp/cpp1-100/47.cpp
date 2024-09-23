#include <cstdio>
#include <queue>
#define upto(i,e) for(int i=1;i<=e;i++)
#define DEBUG false
#define debug if(DEBUG)

int     N;  // Row
int     M;  // Column
char    map[14][14];
bool    visited[14][14];
const int dx[] = { 0, +1, -1,  0,  0};
const int dy[] = { 0,  0,  0, +1, -1};


struct Point {
    int x;
    int y;
    Point(int x, int y) { this->x = x; this->y = y; }
};

bool reachable(Point p) { return 1<=p.x && p.x<=M && 1<=p.y && p.y<=N; }

std::queue<Point> Q;
int main() {
        scanf("%d %d\n", &N, &M);
        upto(y, N) {
            char line[12];
            scanf("%s", line);
            upto(x, M)   map[x][y] = line[x-1];
        }
        debug {
            upto(y, N) {
                upto(x, M)   putchar(map[x][y]);
                putchar('\n');
            }
        }
        Point outset = Point(1, 1);
        upto(y, N) 
            upto(x, M) 
                if (map[x][y] == 'S') {
                    outset = Point(x, y);
                    goto found;
                }
found:  Q.push(outset);
        while (!Q.empty()) {
            Point p = Q.front(); Q.pop();
            if (map[p.x][p.y] == 'T')       goto end;
            if (visited[p.x][p.y])          continue;
            visited[p.x][p.y] = true;
            debug printf("(%d, %d)\n", p.x, p.y);
            upto(i, 4) {
                Point t = {p.x+dx[i], p.y+dy[i]};
                if (!reachable(t))          continue;
                debug printf("(%d, %d) %c\n", t.x, t.y, map[t.x][t.y]);
                if (map[t.x][t.y] == '*')   continue;
                Q.push(t);
            }
        }
        printf("no\n"); return 0;
end:    printf("yes\n"); return 0;
}