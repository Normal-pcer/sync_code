#include <cstring>
#include <iostream>

int main() {
    long double x; std::memset(&x, 0x7f, sizeof(x));
    std::cout << x << std::endl;
    return 0;
}