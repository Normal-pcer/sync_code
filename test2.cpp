#include <bits/stdc++.h>

int main() {
    std::fstream is0("1.out_0", std::ios::in);
    std::fstream is1("1.out_1", std::ios::in);

    std::string s1, s0;
    int cnt = 1;
    while (std::getline(is0, s0) and std::getline(is1, s1)) {
        if (s1 != s0) {
            std::cout << cnt << std::endl;
            return 0;
        }
        cnt++;
    }
}