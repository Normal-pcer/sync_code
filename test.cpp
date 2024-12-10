#include <bits/stdc++.h>

std::vector<int> vec;
std::jmp_buf egg;
void dfs(int dep=0) {
	if (dep == 10) {
		if (std::accumulate(vec.begin(), vec.end(), 0) == 15) {
			std::cout << "goto egg;" << std::endl;
			std::longjmp(egg, 1);
		}
		return;
	}
	vec.push_back(1), dfs(dep+1), vec.pop_back();
	vec.push_back(2), dfs(dep+1), vec.pop_back();
}

int main() {

	if (setjmp(egg)) {
		std::cout << "egg: " << std::endl;
		for (auto i: vec)  std::cout << i << ' ';
		std::cout << std::endl;
		goto egg;
	}

	dfs(0);
egg:
	return 0;
}