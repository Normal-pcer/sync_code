#include <cstdio>
#define range(b, e) for(int i=b; i<=e; i++)


inline int getInt() {
    int temp;
    scanf("%d", &temp);
    return temp;
}
long long count=0;
int N;
////bool map[15][15];           // x, y
bool columnOccupied[15];    // 第i列(x=i)是否被占用
bool b1Occupied[30];        // y=x+(i-15)的常数项是否被占用
bool b2Occupied[30];        // y=-x+(i)的常数项是否被占用
int result[15];

void dfs(int y) {           // 在y上放置棋子
    if (y>N)  { 
        count++;
        printf("%d", result[1]);
        range(2, N)  printf(" %d", result[i]);
        printf("\n");
        return; 
    }
    range(1, N) {
        int x=i;
        if (columnOccupied[x])  continue;
        int b1 = y-x+15;
        int b2 = y+x;
        if (b1Occupied[b1])     continue;
        if (b2Occupied[b2])     continue;

        columnOccupied[x] = true;
        b1Occupied[b1] = true;
        b2Occupied[b2] = true;

            result[y] = x;
            dfs(y+1);

        b2Occupied[b2] = false;
        b1Occupied[b1] = false;
        columnOccupied[x] = false;
    }
}

int main() {
    N = getInt();
    dfs(1);
    printf("%lld\n", count);
    return 0;
}