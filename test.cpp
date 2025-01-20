#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <format>
#include "libs/io.hpp"
using namespace lib;

int main() {
    double x = 0.14514;
    std::printf("printf %.0lf\n", x);
    std::cout << std::fixed << std::setprecision(0) << "cout " << x << std::endl;
    std::cout << std::format("format {:.0f}", x) << std::endl;
    io << "IO: " << x << "\n";
    return 0;
}