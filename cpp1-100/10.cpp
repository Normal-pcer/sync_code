#include <iostream>
#define Infinity (int)1e5+5

int F[Infinity] = {0, 1, 1};

int main(int argc, char const *argv[]) {
    int n;
    std::cin >> n;

    for (int i=3; i<=n; i++) {
        F[i] = (F[i-1] + F[i-2])%2333333;
    }

    std::cout << F[n] << std::endl;
    return 0;
}