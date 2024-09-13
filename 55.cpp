#include <bits/stdc++.h>

int main() {
    int a=2, b=3;
    #define a a+a
    #define b a+a+a
    #define c b+b+b+b
    #define a c+c+c+c+c
    std::cout << a << " " << b << std::endl;
}