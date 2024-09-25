#include <cstdio>
#include <algorithm>
#define initDebug ;
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

#define square(n) ((n)*(n))
#define chkMax(base,cmp) base=std::max(base,cmp)

const int maxN = 52;
struct Point {short r, c, h;} heightMap[maxN * maxN];  // 30.94KB
int strength[maxN * maxN];  // 10.56KB; 从高度为 i 的点向上可能的最高"华丽度"
int N;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    int tempH;
    upto(r, N)  upto(c, N) {
        scanf("%d", &tempH);
        heightMap[tempH] = {(short)r, (short)c, (short)tempH};
    }

    debug upto(i, N*N) printf("(%d, %d, %d) ", heightMap[i].r, heightMap[i].c, heightMap[i].h);
    log("\n")

    int loopTime = N * N;
    for (int i=loopTime; i>=1; i--) {
        int s = 0;
        from(j, i+1, loopTime+1) {  // 更高的点
            chkMax( s, square( abs(heightMap[i].c-heightMap[j].c) + abs(heightMap[i].r-heightMap[j].r) ) + strength[j] );
        }
        strength[i] = s;
        log("%3d", strength[i]);
    }
    log("\n")
    printf("%d\n", strength[1]);
    return 0;
}