#include <bits/stdc++.h>

int main() {
    int a=2, b=3;
    #define a a+a
    #define b a+a+a
    #define c b+b+b+b
    #define a c+c+c+c+c // 覆盖a<=a+a的定义
    std::cout << a << " " << b << std::endl;
/*
a
<=5*c
<=5*4*b
<=5*4*3*a (停止)
b
<=3*a
<=3*5*c
<=3*5*4*b (停止)
*/
}