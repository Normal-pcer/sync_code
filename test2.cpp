#include <bits/stdc++.h>

void swap(int *x, int *y) {
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
}

int main() {
    int a = 5;
    swap(&a, &a);
    std::cout << a << std::endl;
    return 0;
}