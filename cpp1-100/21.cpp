#include <cstdio>
#define MOD 2333333

unsigned long long _2pow(int num) {
    if (num <= 21)  return 1 << num;
    if (num <= 63)  return ((unsigned long long)1 << num) % MOD;
    return (_2pow(num / 2) * _2pow(num - (num / 2))) % MOD;
}

int main(int argc, char const* argv[]) {
    int n;
    scanf("%d", &n);
    int result = 0;
    switch (n) {
    case 0:     result = 0;  break;
    case 1:     result = 3;  break;
    case 2:     result = 6;  break;
    default:    result = (int)(_2pow(n) + 2 * (n % 2 ? -1 : 1)) % MOD;
    }
    printf("%d", result);
    return 0;
}