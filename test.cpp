#include <iostream>

int main() {
	int tot = 5;
	while (auto x = tot) {
		std::cout << x << std::endl;
		tot--;
	}
}