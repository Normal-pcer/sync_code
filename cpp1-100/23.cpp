#include <bits/stdc++.h>

bool isPrime(int number) {
    for (int i = 2; i * i <= number; i++)
        if (!number % i) return false;
    return true;
}
int n, numbers[25];

int main(int argc, char const* argv[]) {

}