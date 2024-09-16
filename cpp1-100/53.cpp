#include <cstdio>
#include <cstring>
#include <queue>
#define upto(i, n) for(int i=1;i<=n;i++)
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define ROWS_MAX 102
#define COLS_MAX 102

struct Vector {
    int x, y;
    Vector () {}
    inline Vector (int x, int y) { this->x = x; this->y = y; }
    inline bool operator==(const Vector&v) const {
        return this->x==v.x && this->y==v.y;
    }
};
struct Position: Vector {
    int turns; Vector direction;
    Position () {}
    inline Position (int x, int y, int turns, Vector direction): Vector(x, y) { this->direction=direction; this->turns=turns; }
    inline Position operator+(const Vector& v) const {
        return {this->x + v.x, this->y + v.y, 
            this->turns + ( ( this->direction==v || (this->direction.x == 0 && this->direction.y == 0) ) ? 0:1 ),
            v};
    }
};

char map[ROWS_MAX][COLS_MAX];
bool walked[ROWS_MAX][COLS_MAX][12][4];
int T, N, M, K, X1, Y1, X2, Y2;
Vector directions[] = {{0, 0}, {0, -1}, {0, 1}, {1, 0}, {-1, 0}};

std::queue<Position> Q;

int search(Position outset, Vector target, int turnsLimit) {
    while (Q.size())      Q.pop();
    std::memset(walked, false, sizeof(walked));
    Q.push(outset);
    
    while (Q.size()) {
        auto now = Q.front();              Q.pop();
        debug printf("(%d, %d) %d (%d, %d)\n", now.x, now.y, now.turns, now.direction.x, now.direction.y);
        upto(i, 4) {
            Position newPos = now + directions[i];
            if (map[newPos.x][newPos.y] != '.')                 continue;
            if (newPos.x > M || newPos.y > N)                   continue;
            if (walked[newPos.x][newPos.y][newPos.turns][i-1])  continue;
            if (newPos.turns > turnsLimit)                      continue;
            if (newPos == target)                               return now.turns;
            walked[newPos.x][newPos.y][newPos.turns][i-1] = true;
            Q.push(newPos);
        }
    }
    return -1;
}

int main() {
    scanf("%d", &T);
    upto(i, T) {
        scanf("%d%d", &M, &N);
        char line[COLS_MAX];
        upto(r, M) {
            scanf("%s", line);
            upto(c, N)      map[r][c] = line[c-1];
        }
        scanf("%d%d%d%d%d", &K, &Y1, &X1, &Y2, &X2);
        printf("%s\n", search({X1, Y1, 0, {0, 0}}, {X2, Y2}, K)==-1?"no":"yes");
    }
}

/*
2
5 5
...**
*.**.
.....
.....
*....
1 1 1 1 3
5 5
...**
*.**.
.....
.....
*....
2 1 1 1 3
*/