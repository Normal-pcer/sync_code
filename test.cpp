#include <bits/stdc++.h>

int main() {
    std::string s, t;
    for (auto i = 0; i < 1e6; i++)  s.push_back('a');
    s.push_back('b');
    for (auto i = 0; i < 1e5; i++)  t.push_back('a');
    t.push_back('b');

    std::cout << (std::search(s.begin(), s.end(), std::boyer_moore_searcher(t.begin(), t.end())) - s.begin()) << std::endl;
}