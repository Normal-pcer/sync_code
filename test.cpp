#include <bits/stdc++.h>

#include "./libs/rolling_container.hpp"


int main() {
    RollingContainer<std::vector<unsigned>, 4> rc(114514);
    for (auto i = 2; i < 1000; i++) {
        rc.at(i) = rc.at(i-1) + rc.at(i-2);
    }
    std::cout << rc.at(114514) << std::endl;
    return 0;
}