#include <cstring>
#include <iostream>

int main() {
    double x = 0.1;
    long long x_ll;
    std::memcpy(&x_ll, &x, sizeof(x));
    std::cout << x_ll << std::endl;
    return 0;
}