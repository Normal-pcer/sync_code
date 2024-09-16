#include <cstdio>
#define DEBUG false
#define range(i, b, e) for(int i=b; i<=e; i++)
#define batchDfs(dx, dy) dfs(x+dx,y+dy);dfs(x-dx,y-dy);dfs(x+dx,y-dy);dfs(x-dx,y+dy);
#define debug if (DEBUG)

int map[102][102]; // i~x; j~y; 0=空，1=障碍，2=走过
int M, N;           // x<=M; y<=N

bool available(int x, int y)  { return 1<=x && x<=M && 1<=y && y<=N; }

void dfs(int x, int y) {
    if (!available(x, y))  return;
    if (map[x][y])  return;
    map[x][y] = 2;
    batchDfs(2, 0);
    batchDfs(1, 1);
    batchDfs(0, 2);
    batchDfs(1, 0);
    batchDfs(0, 1);
}

int graphCount = 0;

int main() {
    scanf("%d%d\n", &N, &M);
    range(i, 1, N) {
        range(j, 1, M) {
            char temp = getchar();
            if (temp=='-')  map[j][i]=1;
        }
        getchar();
    }
    range(i, 1, N)
        range(j, 1, M)
            if (map[j][i]==0) {
                graphCount++;
                dfs(j, i);
                debug {
                    printf("\n===========\nat (%d, %d)\n", j, i);
                    range(k, 1, N) {
                        range(l, 1, M) {
                            putchar(  map[l][k]==0? '#': (map[l][k]==1?'-':'$')  );
                        }
                        putchar('\n');
                    }
                }
            }
    printf("%d\n", graphCount);
    return 0;
}