#include <bits/stdc++.h>
#define mod 12345
#define Infinity 1005

int oddCount[Infinity] = { 0, 1 };  // 奇数
int evenCount[Infinity] = { 1, 8 };  // 偶数
int N;

int main(int argc, char const* argv[]) {
    std::cin >> N;
    for (int i = 2; i <= N; i++) {
        oddCount[i] = (9 * oddCount[i - 1] + evenCount[i - 1]) % mod;
        evenCount[i] = (9 * evenCount[i - 1] + oddCount[i - 1]) % mod;
    }
    std::cout << evenCount[N] << std::endl;
}