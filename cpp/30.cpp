#include <cstdio>

long long trianglesCount(int size) {
    // !完全不对
    long long result = (long long)size * size % 2333333;
    for (int i = 2; i * i < size; i++) {
        if (size % i == 0)
            result += (size / i) * (size / i) % 2333333;
    }
    return result;
}

int main(int argc, char const* argv[]) {
    int n;
    scanf("%d", &n);
    printf("%lld\n", (trianglesCount(n) + 2333333) % 2333333);
    return 0;
}