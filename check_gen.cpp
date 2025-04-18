// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
#include "lib"
#include "libs/range.hpp"
#include "libs/fixed_int.hpp"
    using namespace lib;
#include "randlib.hpp"
    using namespace Random;

    void generate(std::ostream &out) {
        while (true) {
            auto constexpr helperExecutable = "test.exe";
            std::system(std::filesystem::path{helperExecutable}.string().c_str());

            std::fstream fin{std::filesystem::path{u8"2.in"}.string().c_str(), std::ios::in | std::ios::binary};
            std::string s;
            while (std::getline(fin, s)) out << s << '\n';
            
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}