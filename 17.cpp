#include <bits/stdc++.h>

class Position {
public:
    int r;
    int y;

    Position(int x, int y) { this->r = x;  this->y = y; }

    bool operator==(const Position& that) {
        return this->r == that.r && this->y == that.y;
    }

    Position operator+(const Position& that) {
        return Position(this->r + that.r, this->y + that.y);
    }

    void operator+=(const Position& that) {
        this->r += that.r;
        this->y += that.y;
    }
};

Position outset = Position(0, 0);
Position destination = Position(0, 0);
Position occupiedPositions[] = {
    Position(0, 0), Position(1, 2), Position(1, -2), Position(-1, 2), Position(-1, -2),\
    Position(2, 1), Position(2, -1), Position(-2, 1), Position(-2, -1)
};

long long waysOfPoint[21][21];

long long getWays(int x, int y) {
    if (0 <= x && 0 <= y)  return waysOfPoint[x][y];
    else  return 0;
}

long long getWays(Position pos) { return getWays(pos.r, pos.y); }
bool isOccupied(Position pos) {
    for (int i = 0; i < 9; i++)
        if (pos == occupiedPositions[i])
            return true;
    return false;
}

int temp;
int getInt() {
    scanf("%d", &temp);
    return temp;
}

int main(int argc, char const* argv[]) {
    int destinationX = getInt();
    int destinationY = getInt();
    destination += Position(destinationX, destinationY);
    int horseX = getInt();
    int horseY = getInt();
    for (auto& pos : occupiedPositions) {
        pos += Position(horseX, horseY);
    }
    for (auto& pos : occupiedPositions) {
        //// printf("(%d, %d) ", pos.x, pos.y);
    }
    //// printf("\n");

    waysOfPoint[0][0] = 1;
    for (int x = 0; x <= destinationX; x++) {
        for (int y = 0; y <= destinationY; y++) {
            if (x == 0 && y == 0) { continue; }
            if (isOccupied(Position(x, y))) {
                waysOfPoint[x][y] = 0;
                //// printf("  x");
            }
            else {
                waysOfPoint[x][y] = getWays(x - 1, y) + getWays(x, y - 1);
                //// printf("%3d", waysOfPoint[x][y]);
            }
        }
        //// printf("\n");
    }

    printf("%lld\n", getWays(destination));
    //// std::cout << getWays(destination) << std::endl;
    return 0;
}
