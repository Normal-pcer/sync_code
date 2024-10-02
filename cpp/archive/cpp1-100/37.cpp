#include <cstdio>


__int128 N;

int main() {
    unsigned long long n = 18446744073709551615ULL;
    N=(__int128)n + 2;
    printf("%lld\n", (long long)N%0x7fffffffffffffff);
    return 0;
}