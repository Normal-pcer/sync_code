#include <bits/stdc++.h>

int n;

int main(int argc, char const* argv[]) {
    std::cin >> n;
    switch (n) {
    case 1:
        std::cout << 3 << std::endl; break;
    case 2:
        std::cout << 6 << std::endl; break;
    case 3:
        std::cout << 6 << std::endl; break;
    default:
        long long result = 9;
        for (int i = 0; i < n - 3; i++)  result = (result * 2) % 2333333;
        std::cout << result << std::endl;
    }
    return 0;
}