// Do not expand include
#include "./lib_v2.hpp"
#include "./libs/range.hpp"

using namespace lib;

constexpr const char *InferiorFileName = "check_arena0.cpp";
constexpr const char *SolutionFileName = "check_arena1.cpp";
constexpr const char *InputFileName = "1" ".in";
constexpr const char *OutputFileName = "1" ".out";
constexpr const char *CompileCommand = "g++ {} -o {} -std=c++23 -O2 -Wall -Wextra -Wl,-stack=2147483647";
constexpr const int MaxTimes = inf;

#include "check_gen.cpp"

namespace Checker {
    void generate(std::fstream &out) {
        Generator::generate(out);
    }

    void pause() {
        std::cout << "Press any to continue, 'q' to exit..." << std::endl;
        char ch = getchar();
        if (ch == 'q')  std::exit(0);
    }

    void process(std::string &s) {
        auto i = s.end() - 1;
        for (; *i == ' '; i--);
        s.erase(i+1, s.end());
    }

    void clearBack(std::vector<std::string> &vec) {
        if (vec.empty())  return;
        auto i = vec.end() - 1;
        for (; i->empty(); i--);
        vec.erase(i+1, vec.end());
    }

    std::string displayChar(char ch) {
        if (32 < ch and ch < 127)  return std::format("{}", ch);
        else  return std::format("(ASCII {})", (int)ch);
    }

    std::string defaultCheck(std::vector<std::string> &lines1, std::vector<std::string> &lines2) {
        if (lines1.size() < lines2.size())  return "Read too few lines.";
        if (lines1.size() > lines2.size())  return "Read too many lines.";

        for (auto i = (size_t)0; i != lines1.size(); i++) {
            std::string &l1 = lines1.at(i), &l2 = lines2.at(i);
            if (l1.size() < l2.size())  return std::format("Too short on line {}.", i+1);
            if (l1.size() > l2.size())  return std::format("Too long on line {}.", i+1);

            for (auto j = (size_t)0; j != l1.size(); j++) {
                if (l1.at(j) != l2.at(j)) {
                    return std::format("On line {} column {}: read {}, expect {}.", i+1, j+1, displayChar(l1.at(j)), displayChar(l2.at(j)));
                }
            }
        }
        return "";
    }

    std::string check(std::vector<std::string> &lines1, std::vector<std::string> &lines2) {
        // return defaultCheck(lines1, lines2);
        if (lines1.size() < lines2.size())  return "Read too few lines.";
        if (lines1.size() > lines2.size())  return "Read too many lines.";

        for (auto i = (size_t)0; i != lines1.size(); i++) {
            std::string &l1 = lines1.at(i), &l2 = lines2.at(i);
            std::stringstream s1(l1), s2(l2);

            std::vector<double> vec1, vec2; 
            double tmp;
            while (s1 >> tmp)  vec1.push_back(tmp);
            while (s2 >> tmp)  vec2.push_back(tmp);

            if (vec1.size() > vec2.size())  return std::format("Read too many numbers on line {}.", i+1);
            if (vec1.size() < vec2.size())  return std::format("Read too few numbers on line {}.", i+1);

            const double eps = 1e-4;
            for (auto j = (size_t)0; j != vec1.size(); j++) {
                auto num1 = vec1.at(j), num2 = vec2.at(j);
                if (abs(num1 - num2) <= eps)  continue;
                if (abs(num1 - num2) / num2 <= eps)  continue;
                std::string suffix = "th";
                if ((j+1) % 10 == 1 and (j+1) % 100 != 11)  suffix = "st";
                if ((j+1) % 10 == 2 and (j+1) % 100 != 12)  suffix = "nd";
                if ((j+1) % 10 == 3 and (j+1) % 100 != 13)  suffix = "rd";
                return std::format("On {}{} number of line {}: read {}, expect {} (eps = {}).", j+1, suffix, i+1, num1, num2, eps);
            }
        }
        return "";
    }

    std::string compare(std::vector<std::string> &lines1, std::vector<std::string> &lines2) {
        std::for_each(lines1.begin(), lines1.end(), process);
        std::for_each(lines2.begin(), lines2.end(), process);

        clearBack(lines1), clearBack(lines2);
        return check(lines1, lines2);
    }

    char tmp[1024];
    void compile() {
        *std::format_to(tmp, CompileCommand, InferiorFileName, "inferior.exe") = 0;
        std::cout << tmp << std::endl;
        if (std::system(tmp))  throw "inferior compile error";

        *std::format_to(tmp, CompileCommand, SolutionFileName, "solution.exe") = 0;
        std::cout << tmp << std::endl;
        if (std::system(tmp))  throw "solution compile error";
    }

    void main() {
        compile();

        for (auto i = 0; i != MaxTimes; i++) {
            std::fstream in(InputFileName, std::ios::out | std::ios::trunc);
            if (not in.is_open())  throw "failed to open input";
            generate(in);
            in.close();

            std::string line;
            std::vector<std::string> lines1, lines2;

            *std::format_to(tmp, "inferior.exe < {} > {}", InputFileName, OutputFileName) = 0;
            auto begin = std::chrono::system_clock::now();
            int ret1 = std::system(tmp);
            auto end = std::chrono::system_clock::now();
            std::fstream f1(OutputFileName, std::ios::in);
            while (std::getline(f1, line))  lines1.push_back(line);
            f1.close();

            auto duration = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000;

            *std::format_to(tmp, "solution.exe < {} > {}", InputFileName, OutputFileName) = 0;
            std::system(tmp);
            std::fstream f2(OutputFileName, std::ios::in);
            while (std::getline(f2, line))  lines2.push_back(line);
            f2.close();

            if (ret1)  std::cout << "Runtime Error On test " << i << " with value " << ret1 << std::endl, pause();
            std::string res = compare(lines1, lines2);
            if (not res.empty()) {
                std::cout << std::format("Wrong Answer on test {} in {:.2f} sec", i, duration) << std::endl;
                std::cout << res << std::endl;
                pause();
            } else {
                std::cout << std::format("Accepted on test {} in {:.2f} sec", i, duration) << std::endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    try {
        Checker::main();
    } catch (const char *str) {
        std::cerr << "terminate called after throwing an instance of 'char const*': " << std::endl;
        std::cerr << str << std::endl;
    }

    return 0;
}