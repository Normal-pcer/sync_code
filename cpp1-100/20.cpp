#include <bits/stdc++.h>
#define mod 2333333

int f[] = { 1, 3, 11 };
//       i -2 -1   0  

int main(int argc, char const* argv[]) {
    int n;
    std::cin >> n;

    if (n % 2) { std::cout << 0 << std::endl; return 0; }
    n = n / 2;
    for (int i = 2; i < n; i++) {
        f[0] = f[1];
        f[1] = f[2];
        f[2] = (4 * f[1] - f[0]) % mod;
    }
    if (n <= 2)  std::cout << f[n] << std::endl;
    else  std::cout << (f[2] + mod) % mod << std::endl;
    return 0;
}