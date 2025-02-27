#include <bits/stdc++.h>

struct A {
private:
    int y = 5;
public:
};

int main() {
    A a;
    A::B b{&a};
    b.f();
}