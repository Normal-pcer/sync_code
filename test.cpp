#include <iostream>

struct A {
  A() { std::cout << "A"; }
};
struct B {
  B() { std::cout << "B"; }
};

class C {
public:
    C(int _a, int _b) : a(_b), c(_a) {

    }

private:
    int a, b, c;
};

int main()
{
    C(1, 2);
}