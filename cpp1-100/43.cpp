#include <cstdio>
#include <algorithm>
#define DEBUG false
#define range(i, b, e) for(int i=b; i<=e; i++)
#define batchDfs(dx, dy) dfs(x+dx,y+dy)+dfs(x-dx,y-dy)+dfs(x+dx,y-dy)+dfs(x-dx,y+dy)
#define debug if (DEBUG)

const char available = '*';
char map[1002][1002]; // i~x, j~y; .~Road, *~barrier;
int W, H;           // Width~x, Height~y;
int answer = 0;

// returns size of this chunk
int dfs(int x, int y) {
    if (map[x][y] != available)  return 0;
    map[x][y] = ',';
    return batchDfs(0, 1) + batchDfs(1, 0) + 1;
}

int main() {
    scanf("%d\n", &W);
    H = W;
    range(y, 1, H) {
        range(x, 1, W)   map[x][y] = getchar();
        getchar();  // Ignore Linefeed Symbol
    }
    int count = 0;
    range(y, 1, H) {
        range(x, 1, W) {
            if (map[x][y] == available) {
                int size = dfs(x, y);
                answer = std::max(answer, size);
                count++;
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
    printf("%d\n", count);
    return 0;
}