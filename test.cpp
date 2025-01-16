#include <bits/stdc++.h>

void f(auto &&x, int cnt = 5) {
    if (cnt == 0)  return;
    std::cout << ": " << x() << " " << sizeof(x) << std::endl;
    f(x, cnt - 1);
}

int g() {
    return 1;
}

int main() {
    int x = 5, y = 2;
    f([=]() { return x + y; });
    // f(g);
    return 0;
}
