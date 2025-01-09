#include <bits/stdc++.h>

int main() {
    while (true) {
        double x = 0;
        std::cin >> x;
        char *pt;
        pt = reinterpret_cast<char *>(&x);
        for (auto _ = 7; _ >= 0; _--) {
            pt = reinterpret_cast<char *>(&x) + _;
            std::cout << std::bitset<8>(*pt) << std::endl;
            pt++;
        }

        unsigned long long y = *reinterpret_cast<unsigned long long *>(&x);
        std::cout << !!(y & (1LL << 63)) << std::endl;
        std::cout << ((((y >> 52) & ~(1LL << 10)) + 13) & 1023) << std::endl;
        std::cout << (y & ((1LL << 52) - 1)) << std::endl;

        auto digits = ((((y >> 52) & ~(1LL << 10)) + 13) & 1023);
        auto res = (y & ((1LL << digits) - 1));
        std::cout << *reinterpret_cast<double *>(&res) << std::endl;
    }
}