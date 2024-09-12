#include <cstdio>
#include <algorithm>
#define DEBUG false
#define range(i, b, e) for(int i=b; i<=e; i++)
#define batchDfs(dx, dy) dfs(x+dx,y+dy);dfs(x-dx,y-dy);dfs(x+dx,y-dy);dfs(x-dx,y+dy);
#define debug if (DEBUG)

char map[1002][1002];   // i~x, j~y; .~水, #~船, +~遍历完成
int R, C;               // x~Columns, y~Rows
int minX = 2147483647;
int minY = 2147483647;
int maxX = 0;
int maxY = 0;
int size = 0;

void dfsInit() {
    minX = 2147483647;
    minY = 2147483647;
    maxX = 0;
    maxY = 0;
    size = 0;
}

void dfs(int x, int y) {
    if (map[x][y] != '#')  return;
    minX = std::min(minX, x);
    maxX = std::max(maxX, x);
    minY = std::min(minY, y);
    maxY = std::max(maxY, y);
    size++;
    map[x][y] = '+';
    batchDfs(1, 0);
    batchDfs(0, 1);
}

int main() {
    scanf("%d %d\n", &R, &C);
    range(y, 1, R) {
        range(x, 1, C)  map[x][y] = getchar();
        getchar();  // 换行符
    }
    int count=0;
    range(y, 1, R)
        range(x, 1, C) 
            if (map[x][y] == '#') {
                count++;
                dfsInit();
                dfs(x, y);
                if ((maxX-minX+1) * (maxY-minY+1) != size) {
                    goto end;
                }
            }
    printf("There are %d ships.\n", count);     return 0;
end:
    printf("Bad placement.\n");                 return 0;
}
