#include <stdio.h>

typedef struct Test {
    int x;
};

struct Test func() {
    struct Test res;
    res.x = 5;
    return res;
}

int main() {
    
}