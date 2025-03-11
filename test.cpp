#include <bits/stdc++.h>

int main() {
    auto size = (1uz << 34) >> 3;
    size_t cnt = 0;
    while (true) {
        auto ptr = static_cast<unsigned long long *>(std::malloc(size * sizeof(unsigned long long)));
        for (size_t i = 0; i < size; i++) {
            if (i * 32 % (1ULL << 32) == 0)  std::cout << ((++cnt) * 256) << "MB" << std::endl;
            ptr[i] = i;
        }
    }
    // unsigned long long sum = 0;
    // for (size_t i = 0; i < size; i++) {
        // sum += ptr[i];
    // }
    // std::cout << sum << std::endl;
    return 0;
}