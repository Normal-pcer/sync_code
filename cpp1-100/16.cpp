#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);
    long long result = 3;
    for (int i = 2; i < n; i++) {
        result *= 2;
        result %= 2333333;
    }
    if (n==2)  printf("6");
    else  printf("%lld\n", result);
    return 0;
}