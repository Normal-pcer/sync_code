#include <bits/stdc++.h>

int main() {
    char arr[] = "鸡\0\0\0";
    std::cout << *(int *)arr << std::endl;
    return 0;
}