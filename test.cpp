#include <bits/stdc++.h>

std::jmp_buf label;

void check(int x, int p) {
    if (p * p > x)  return;
    if (x % p == 0)  std::longjmp(label, true);
    check(x, p+1);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N;  std::cin >> N;
    auto cnt = 0, i = 1;
    while (cnt != N) {
        i++;
        check(i, 2);
        cnt++;
        setjmp(label);
    }
    std::cout << i << std::endl;
    return 0;
}