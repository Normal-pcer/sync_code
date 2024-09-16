#include <cstdio>

inline long long safeGet(long long* array, int index) {
    if (index < 0 || index >= 55)  return 0;
    return *(array + index);
}

long long a[55] = { 1, 2 };

int main(int argc, char const* argv[]) {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; i++) {
        a[i] = 2 * safeGet(a, i - 1) - (long long)(i == m ? 1 : safeGet(a, i - m - 1));
         //// printf("[%d] %lld\n", i, a[i]);
    }
    printf("%lld\n", a[n]);
    return 0;
}