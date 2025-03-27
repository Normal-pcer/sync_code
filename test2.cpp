#include "./libs/null_number.hpp"

using namespace lib;
int main() {
    double x = null;
    std::cout << x << std::endl;
    std::cout << (x == null) << std::endl;
}