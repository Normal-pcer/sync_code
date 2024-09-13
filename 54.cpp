#include <bits/stdc++.h>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define MAX_SIZE 102

char map[MAX_SIZE][MAX_SIZE];
bool walked[MAX_SIZE][MAX_SIZE];
int M, N;
const int dx[] = { 0,  0,  0, +1, -1};
const int dy[] = { 0, -1, +1,  0,  0};

struct Position {
    int x, y, coins;
    bool waiting=false;
    char from='\0';
    inline bool operator==(const Position& p) const { return this->x==p.x && this->y == p.y; }
};

Position target;
std::deque<Position> Q;
int search() {
    Q.push_back({1, 1, 0});
    while ( Q.size() ) {
        auto p=Q.front();       Q.pop_front();
        if (walked[p.x][p.y])   continue;
        debug printf("(%d, %d) %d %d\n", p.x, p.y, p.coins, p.waiting);
        if (p.waiting) {
            p.waiting = false;
            Q.push_back(p);
            continue;
        }
        if (p == target)        return p.coins;
        walked[p.x][p.y] = true;
        upto(i, 4) {
            Position n = {p.x+dx[i], p.y+dy[i], p.coins+1};
            if (n.x<=0 || n.x>M || n.y<=0 || n.y>M) continue;
            if (map[n.x][n.y] == map[p.x][p.y]) {
                if (map[p.x][p.y] == '\0')  { continue; }
                else { n.coins--; Q.push_front(n); continue; }
            }
            if (map[p.x][p.y] == '\0') {
                if (map[n.x][n.y] == '\0')  continue;
                else { n.coins-=( p.from == map[n.x][n.y] ); Q.push_front(n); continue; }
            } else {
                if (map[n.x][n.y] == '\0') { 
                    n.coins++;
                    n.waiting = true;
                    n.from = map[p.x][p.y];
                    Q.push_back(n); 
                    continue; 
                } else {
                    if (map[p.x][p.y] == map[n.x][n.y]) { n.coins--; Q.push_front(n); continue; }
                }
            }
            Q.push_back(n);
        }
    }
    return -1;
}

int main() {
    scanf("%d%d", &M, &N);
    int x, y; char to;
    upto(_, N) {
        scanf("%d %d %c", &x, &y, &to);
        map[x][y] = to;
    }
    target = {M, M, 0};
    printf("%d\n", search());
    return 0;
}

/*
5 7
1 1 0
1 2 0
2 2 1
3 3 1
3 4 0
4 4 1
5 5 0
*/