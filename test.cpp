#include <bits/stdc++.h>

int main() {
	std::fstream out("sum.in");
	std::mt19937 rng{std::random_device{}()};

	auto n = 10000, m = 100, k = 100;
	out << n << ' ' << m << ' ' << k << std::endl;
	for (auto i = 0; i < n; i++) {
		out << std::uniform_int_distribution{0, 10000}(rng) << " ";
	}
	out << std::endl;
}