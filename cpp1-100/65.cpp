#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1) && !strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)

bool DEBUG_MODE = false;

const int MAX_SIZE = 1002;
const int MOD = (int)1e9+7;

char map[MAX_SIZE][MAX_SIZE];
int count[MAX_SIZE][MAX_SIZE];
int H, W;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    scanf("%d%d", &H, &W);
    char line[MAX_SIZE];
    upto(r, H) {
        scanf("%s", line);
        upto(c, W)  map[r][c] = line[c-1];
    }
    count[1][1] = 1;
    upto(r, H) {
        upto (c, W) {
            if (map[r][c] == '#')   continue;
            //// if (r==1 || c==1) { count[r][c] = 1; continue; }
            count[r][c] += (count[r][c-1] + count[r-1][c]) % MOD;
        }
    }
    debug {
        upto(r, H) {
            upto (c, W) {
                printf("%4d", count[r][c]);
            }
            printf("\n");
        }
    }
    printf("%d\n", count[H][W]);
    return 0;
}