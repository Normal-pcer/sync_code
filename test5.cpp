#include <bits/stdc++.h>

struct A {
	int x, y;
};

int main() {
	std::vector<A> v;
	std::ranges::sort(v.begin(), v.end(), std::less{}, [&](A a) { return std::pair{a.x, a.y}; });
}