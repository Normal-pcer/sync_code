#include <cstdio>
#include <iostream>
#define DEBUG false
#define debug if (debug) 
#define loop(i,b,e) for(int i=b;i<e;i++)
// !  loop(i, b, e)不会取到e

bool row[9][9];         // 第 i 行中是否已经存在数字 j
bool column[9][9];
bool place[9][9];

int map[9][9];
int* firstUnsolved;     // 指向map中的首个空位（即0）

struct MutableArray {
    int topIndex;
    int data[9];
    MutableArray() { this->topIndex = 0; }
    void push(int num) { this->data[++topIndex] = num; }
};

int getPlace(int x, int y) {
    return ( y / 3 * 3 ) + ( x / 3 );
}

int set(int x, int y, int value) {
    map[x][y] = value;
    if (value) {
        row[y][value] = true;
        column[x][value] = true;
        place[getPlace(x, y)][value] = true;
    }
    return value;
}

int unset(int x, int y) {
    int value = map[x][y];
    map[x][y] = 0;
    row[y][value] = false;
    column[x][value] = false;
    place[getPlace(x, y)][value] = false;
    return value;
}

MutableArray getZeroIndex(int* at) {
    auto result = MutableArray();
    loop(i, 0, 9) 
        if (*(at+i) == 0) 
            result.push(i);
    return result;
}

bool solve() {
}

int main() {
    int temp;
    loop(y, 0, 9) {
        loop(x, 0, 9) {
            scanf("%d", &temp);
            set(x, y, temp);
        }
    }
}



/*
012345678

005060100
200104009
300000002
802070401
006000200
509010603
600000005
400609007
008040300
 */