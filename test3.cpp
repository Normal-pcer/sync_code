#include <bits/stdc++.h>

using f80 = long double;
using f64 = double;
using u64 = unsigned long long;
using i64 = long long;

int const N = 2e6;
std::array<f64, N> arr1;
std::array<f64, N> arr2;

int main() {
	std::mt19937_64 rng{std::random_device{}()};
	for (int i = 0; i < N; i++) {
		arr1[i] = rng();
		arr2[i] = rng();
	}

	auto begin1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		arr1[i] = arr1[i] / (arr1[i] - 1);
	}
	auto end1 = std::chrono::high_resolution_clock::now();

	u64 res = 0;
	for (int i = 0; i < N; i++) {
		u64 tmp;  std::memcpy(&tmp, &arr1[i], sizeof(u64));
		res ^= tmp;
	}
	std::cout << res << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - begin1).count() << std::endl;

	begin1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		arr2[i] = std::sqrt(arr2[i] - 1);
	}
	end1 = std::chrono::high_resolution_clock::now();

	res = 0;
	for (int i = 0; i < N; i++) {
		u64 tmp;  std::memcpy(&tmp, &arr2[i], sizeof(u64));
		res ^= tmp;
	}

	std::cout << res << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - begin1).count() << std::endl;
}