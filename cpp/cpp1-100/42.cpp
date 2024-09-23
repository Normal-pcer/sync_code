#include <cstdio>
#include <algorithm>
#define DEBUG false
#define range(i, b, e) for(int i=b; i<=e; i++)
#define batchDfs(dx, dy) dfs(x+dx,y+dy)+dfs(x-dx,y-dy)+dfs(x+dx,y-dy)+dfs(x-dx,y+dy)
#define debug if (DEBUG)

char map[752][752]; // i~x, j~y; .~Road, *~barrier;
int W, H;           // Width~x, Height~y;
int answer = 0;

// returns size of this chunk
int dfs(int x, int y) {
    if (map[x][y] != '.')  return 0;
    map[x][y] = ',';
    return batchDfs(0, 1) + batchDfs(1, 0) + batchDfs(1, 1) + 1;
}

int main() {
    scanf("%d%d\n", &W, &H);
    range(y, 1, H) {
        range(x, 1, W)   map[x][y] = getchar();
        getchar();  // Ignore Linefeed Symbol
    }
    range(y, 1, H) {
        range(x, 1, W) {
            if (map[x][y] == '.') {
                int size = dfs(x, y);
                answer = std::max(answer, size);
                debug {
                    range(y, 1, H) {
                        range(x, 1, W) {
                            putchar(map[x][y]);
                        }
                        putchar('\n');
                    }
                }
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}