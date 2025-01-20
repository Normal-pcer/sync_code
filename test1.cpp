#include <bits/stdc++.h>

const int _N = 10008;
std::bitset<_N> not_prime = 3;
std::vector<int> primes;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N;  std::cin >> N;
    for (auto i = 2; i < N + 1; i++) {
        if (not not_prime[i])  primes.push_back(i);
        for (auto p: primes) {
            not_prime[p * i] = true;
            if (i % p == 0)  break;
        }
    }

    for (auto p: primes)  std::cout << p << " ";
    std::cout << std::endl;
}