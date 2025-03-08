#include <bits/stdc++.h>

int main() {
    char const str[] = {'d', static_cast<char>(105), 'c', 'k', '\0'};
    std::int32_t x;  std::memcpy(&x, str, 4);

    std::cout << (2 ^ x) << std::endl;
    return 0;
}