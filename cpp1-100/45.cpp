#include <cstdio>
#define loop(i, b, e) for(int i=b; i<=e; i++)
#define DEBUG true
#define debug if (DEBUG)
#define batchDfs(dx, dy, v) (dfs(x+dx,y+dy, v) && dfs(x-dx,y-dy, v) && dfs(x+dx,y-dy, v) && dfs(x-dx,y+dy, v))

int N;      // Count of rows
int M;      // Count of columns
int height[752][752];
int walked[752][752];

struct Point {
    int x;
    int y;
    Point(int x, int y) { this->x = x; this->y = y; }
};

inline int getHeight(Point p) { return height[p.x][p.y]; }
inline int getWalked(Point p) { return walked[p.x][p.y]; }

inline bool dfsP(Point p);
// Returns - (x, y) is a legal "hilltop" or not
//// Always returns true if height of (x, y) is lower than $value, or false if height of (x, y) is greater than $value 
bool dfs(int x, int y) {
    if (x<=0 || x>M || y<=0 || y>M)     return true;
    if (walked[x][y])                   return true;
    int h = height[x][y];
    walked[x][y] = true;
    Point neighbors[] = { Point(x+1, y), Point(x-1, y), Point(x, y-1), Point(x, y+1) };
    Point extra[] = { Point(x+1, y+1), Point(x+1, y-1), Point(x-1, y-1), Point(x-1, y+1) };
    bool result = true;
    loop(i, 0, 3)   result = result && getHeight(neighbors[i]) <= h;
    loop(i, 0, 3)   result = result && (getHeight(extra[i]) != h || dfsP(extra[i]));
    walked[x][y] = false;
    return result;
}

int main() {
    scanf("%d %d\n", &N, &M);
    loop(y, 1, N)   loop(x, 1, M)   scanf("%d", &height[x][y]);
    int count = 0;
    debug {
        printf("%d\n", dfs(2, 7));
    }
    loop(y, 1, N) 
        loop(x, 1, M)
            if (!walked[x][y] && !height[x][y] && dfs(x, y)) {
                debug {printf("(%d, %d)\n", x, y);}
                count++;
            }
    printf("%d\n", count);
    return 0;
}

inline bool dfsP(Point p) { return dfs(p.x, p.y); }

/*
8 7
4 3 2 2 1 0 1
3 3 3 2 1 0 1
2 2 2 2 1 0 0
2 1 1 1 1 0 0
1 1 0 0 0 1 0
0 0 0 1 1 1 0
0 1 2 2 1 1 0
0 1 1 1 2 1 0
*/