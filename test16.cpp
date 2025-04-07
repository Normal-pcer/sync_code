#include <bits/stdc++.h>

int main() {
    std::string s;
    std::cin >> s;

    std::multiset<char> ms;
    for (auto x: s) ms.insert(x);

    for (auto x: ms) {
        std::cout << x << ": " << ms.count(x) << std::endl;
    }
}
//plnxceoqrhmptuwklerpuwr
