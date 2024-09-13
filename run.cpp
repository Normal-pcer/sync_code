#include <cstdlib>
#include <fstream>
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
    bool defaultDebug = false;
    std::ifstream configIn ("config.txt");
    configIn >> defaultDebug;
    bool noDebug = (argc-2) && !(strcmp("-nd", argv[2]) && strcmp("--nd", argv[2]));
    bool debug = (argc-2) && !(strcmp("-d", argv[2]) && strcmp("--d", argv[2]));
    bool setDebug = (argc-2) && !strcmp("--d", argv[2]);
    bool setNoDebug = (argc-2) && !strcmp("--nd", argv[2]);
//    printf("%d %d\n", setDebug, setNoDebug);
    bool finalDebug = (noDebug||debug)?debug:defaultDebug;
    std::string filename = std::string(argv[1]);
    std::string command = "g++ " + filename + ".cpp -o " + filename + ".exe -std=c++20 -O2 -Wall -Wextra";
    for (int i=3; i<argc; i++) {
        command += " "+std::string(argv[i]);
    }
    system(command.c_str());
    if (debug)  printf("============DEBUG==========\n");
    else        printf("===========================\n");
    std::string cmd = ".\\" + filename + ".exe";
    if (finalDebug)  cmd+=" -d";
    int statusCode = system(cmd.c_str());
    printf("==== Stopped with code %d ====\n", statusCode);
    if (setNoDebug || setDebug) {
        std::ofstream configOut ("config.txt");
        configOut << (debug?"1":"0");
    }
    return 0;
}