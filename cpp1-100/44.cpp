#include <cstdio>
#include <algorithm>
#define DEBUG false
#define range(i, b, e) for(int i=b; i<=e; i++)
#define batchDfs(dx, dy, dz) dfs(x+dx,y+dy,z+dz);dfs(x-dx,y-dy,z+dz);dfs(x+dx,y-dy,z+dz);dfs(x-dx,y+dy,z+dz);dfs(x+dx,y+dy,z-dz);dfs(x-dx,y-dy,z-dz);dfs(x+dx,y-dy,z-dz);dfs(x-dx,y+dy,z-dz)
#define debug if (DEBUG)

char map[102][102][102];
int size;

void dfs(int x, int y, int z) {
    if (map[x][y][z] != '*')  return;
    map[x][y][z] = ',';
    batchDfs(0, 0, 1); batchDfs(0, 1, 0); batchDfs(1, 0, 0);
}

int main() {
    scanf("%d\n", &size);
    range(i, 0, size*size-1) {
        range(z, 1, size) {
            int x = i%size + 1;
            int y = i/size + 1;

            map[x][y][z] = getchar();
        }
        getchar();
    }
    int count = 0;
    range(x, 1, size) {
        range(y, 1, size) {
            range(z, 1, size) {
                if (map[x][y][z] == '*') {
                    dfs(x, y, z);
                    count++;
                }
            }
        }
    }
    printf("%d\n", count);
    return 0;
}