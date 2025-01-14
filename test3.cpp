#include <bits/stdc++.h>

struct A {
    int a, b;
};

A f() { return {}; }

int main() {
    f() = A{1, 2};
    return 0;
}