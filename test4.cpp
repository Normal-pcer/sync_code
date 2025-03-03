#include <bits/stdc++.h>

int main() {
    std::string s0, s1;
    std::fstream fs0("1.out_0", std::ios::in);
    std::fstream fs1("1.out_1", std::ios::in);

    int cnt = 0;
    while (std::getline(fs0, s0) and std::getline(fs1, s1)) {
        cnt++;
        if (s0 != s1)  std::cout << cnt << std::endl;
    }
    return 0;
}