#include <bits/stdc++.h>

struct A {
    auto f() -> void { std::cout << "111" << std::endl; }
};

struct B {
    auto virtual f() -> void = 0;
};

struct Z: public A, public B {
    using A::f;
    using B::f;
};

int main() {
    Z x;
    x.f();
}