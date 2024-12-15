#include "lib"
using namespace lib;

int main() {
    std::vector vec{0, 1, 2, 3};
    ranges::transform(vec, vec.begin(), lam(x, x+1));
}