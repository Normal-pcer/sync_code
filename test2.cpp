#include <bits/stdc++.h>

int main() {
    int x = 0x12345678;
    char *pt = reinterpret_cast<char *>(&x);
    pt++;
    int y = 0;
    std::memcpy(&y, pt, sizeof(int) - 1);
    std::cout << y << std::endl;
    std::cout << 0x123456 << std::endl;
    return 0;
}