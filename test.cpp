#include <iostream>
#include <csignal>
#include <cmath>

auto handle(int) -> void {
    std::cout << "handle" << std::endl;
}

int main() {
    std::signal(SIGFPE, handle);
    double a;  std::cin >> a;
    std::cout << std::sqrt(a) << std::endl;
}