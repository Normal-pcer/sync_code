#include <iostream>
#define Infinity (int)1e5+5
#define mod 2333333

int f[Infinity]={1, 3}, n;

int main(int argc, char const *argv[]) {
    std::cin >> n;
    
    for (int i=2; i<=n; i++) {
        /**
         * 2f[i-1] - 最后一个字符无论是什么，填上E或F都是「安全」的
         * 2f[i-2] - 或者也可以从倒数第二个开始，填EO或FO
         */
        f[i] = (2*f[i-1] + 2*f[i-2]) % mod;
    }

    std::cout << f[n] << std::endl;
    return 0;
}