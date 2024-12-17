#include <bits/stdc++.h>
namespace lib {
    template <typename T, const int p>
    class Mod {
    public:
        T data;

        Mod() = default;
        Mod(T num) : data(positive(num % p)) {}

        T positive(T num) { 
            if (num>=0)  return num;
            if (num+p>=0)  return num+p;
            return num + p * ((-num) / p + 1);
        }

        Mod operator+(const Mod &other) { return Mod(positive(data + other.data) % p); }
        Mod operator-(const Mod &other) { return Mod(positive(data - other.data) % p); }
        Mod operator*(const Mod &other) { return Mod(positive(data * other.data) % p); }
        Mod operator/(const Mod &other) { return Mod((data * other.data)); }
    };
}