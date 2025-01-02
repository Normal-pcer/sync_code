#include <bits/stdc++.h>

struct A {
    A() = default;
    A(const A &) = delete;
};

struct B: public A {
    int x;
    B(int x): x(x) {}
};

int main() {
}