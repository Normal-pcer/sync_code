#include <cstdlib>
#include <string>
#include <windows.h>
#include <regex>
#include <cstdio>
#include <ctime>

inline std::string join(std::string connector, char** begin, char** end) {
    std::string result = *begin;
    while (++begin != end) {
        result += connector + *begin;
    }
    return result;
}

int main(int argc, char const* argv[]) {
    std::string filename = std::string(argv[1]);
    std::string command = "g++ " + filename + ".cpp -o " + filename + ".exe -std=c++20 -O2 -Wall";
    system(command.c_str());
    printf("===========================\n");
    int statusCode = system((".\\" + filename + ".exe").c_str());
    printf("==== Stopped with code %d ====\n", statusCode);
    return 0;
}