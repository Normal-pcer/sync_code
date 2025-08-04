#include <chrono>

unsigned f(unsigned x) {
	auto y = std::chrono::system_clock::now().time_since_epoch().count();
    return x + y;
}

unsigned g(unsigned x) {
    unsigned res = 0;
    for (unsigned i = 0; i < x; ++i) {
        res += f(i);
    }
    return res;
}
