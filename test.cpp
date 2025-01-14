#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
const char s[] = "做个文明中国人ccccc🤣🤣🤣🤣❤❤❤😡😡😡";


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    for (auto *it = reinterpret_cast<const unsigned char *>(std::begin(s)); *it != '\0'; ) {
        const unsigned int x = *it;
        std::cout << std::bitset<8>(x) << std::endl;
        int byte_cnt = (x >> 7? __builtin_clz(~x << 24): 1);
        for (int i = 0; i < byte_cnt; i++)  std::cout << *it++;
        std::cout << std::endl;
    }
    getchar();
    return 0;
}