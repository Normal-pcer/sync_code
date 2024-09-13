#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1) && !strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)

bool DEBUG_MODE;
const int MAX_SIZE = 1002;
bool map[MAX_SIZE][MAX_SIZE];
int nearestZero[MAX_SIZE][MAX_SIZE];
int N, M;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> N >> M;
    upto(r, N) {
        upto(c, M) {
            scanf("%d", &map[r][c]);
        }
    }
    int ans=0;
    upto(r, N) {
        upto(c, M) {
            if (!map[r][c]) {
                upto(i, N) {
                    upto(j, M) {
                        nearestZero[i][j] = std::max( abs(i-r), abs(j-c) );
                    }
                }
            }
        }
    }
    upto(r, N) {
        upto(c, M) {
            if (!map[r][c]) {
                // ans = std::max(ans, 2*())
            }
        }
    }
    std::cout << ans << std::endl;
    return 0;
}