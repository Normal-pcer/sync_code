#include <bits/stdc++.h>

int main() {

    std::fstream in("1.in", std::ios::in);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line))  lines.emplace_back(std::move(line));

    auto begin = std::chrono::system_clock::now();
    const std::regex pattern("z[^zpair]*?p.*?a[^zpair]*?i.*?r");

    auto res = 0;
    for (auto const &line: lines) {
        std::smatch match;
        
        auto begin = line.begin(), end = line.end();
        auto cur = 0;
        while (std::regex_search(begin, end, match, pattern)) {
            cur++;
            begin = match[0].second;
        }

        // std::cout << cur << std::endl;

        res ^= cur;
    }

    auto end = std::chrono::system_clock::now();
    std::cout << res << std::endl;

    auto duration = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000;
    std::cout << duration << "sec" << std::endl;

    return 0;
}