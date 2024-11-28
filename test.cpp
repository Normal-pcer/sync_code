#include <bits/stdc++.h>

void swap(int *&p1, int *&p2) {
    std::swap(p1, p2);
}

int main() {
    int a = 5, b = 6;
    int *x = &a, *y = &b;
    swap(x, y);
    std::cout << *x << *y << a << b;
}