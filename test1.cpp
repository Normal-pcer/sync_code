#include <iostream>

int main() {
	int x;  std::cin >> x;
	if (x > 2)
		if (x > 3)  std::cout << 4;
		else  std::cout << 3;
	else
		if (x > 1)  std::cout << 2;
		else  std::cout << 1;
}