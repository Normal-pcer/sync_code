#include <bits/stdc++.h>
struct T;

void foo(T* p) {
    delete p;       // Undefined behavior
}

struct T {
   ~T() { std::cout << 1; }           // Non-trivial destructor
};

int main() {
    T* p = new T;
    foo(p);
    return 0;
}