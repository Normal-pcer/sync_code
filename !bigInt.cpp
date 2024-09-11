#include <string>
#include <cstdlib>
#include <iostream>

class BigInt {
public:
    std::string str;

    BigInt(int number) { this->str = std::to_string(number); }
    BigInt(std::string text) { this->str = text; }

    BigInt operator+(const BigInt other) const {
        std::string command = "python add.py " + this->str + " " + other.str;
        system(command.c_str());
        auto file = freopen("out.txt", "r", stdin);
        std::string output;
        std::cin >> output;
        fclose(file);
        return BigInt(output);
    }
};

int main() {
    auto n1 = BigInt("114514114514114514114514114514114514114514114514");
    auto n2 = BigInt("114514114514114514114514114514114514114514114514");

    std::cout << (n1+n2).str << std::endl;
    return 0;
}

