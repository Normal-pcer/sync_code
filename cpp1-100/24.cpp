// 绷，落题

#include <bits/stdc++.h>
#define Infinity (int)1e5+5
#define mod 2333333

long long highCountAtTime[Infinity] = {1};
long long lowCountAtTime[Infinity] = {0};

int N;
int main(int argc, char const* argv[]) {
    std::cin >> N;

    for (int current = 1; current <= N; current++) {
        long long lastHigh = highCountAtTime[current-1];
        long long lastLow = lowCountAtTime[current-1];
        highCountAtTime[current] = (3*lastHigh + 2*lastLow) % mod;
        lowCountAtTime[current] = (lastHigh + lastLow) % mod;
    }

    printf("%lld %lld\n", highCountAtTime[N], lowCountAtTime[N]);
    return 0;
}