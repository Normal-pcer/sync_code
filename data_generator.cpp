// Do not expand includes
#include <bits/stdc++.h>
#include "libs/fixed_int.hpp"
namespace _Generator {
#include "check_gen.cpp"
}
namespace Generator = _Generator::Generator;

char const constexpr *inputFormat = "{}.in";
char const constexpr *outputFormat = "{}.out";
char const constexpr *dirName = "data_gen_result";
char const constexpr *standardProgram = "check_arena0.exe";
i32 const constexpr dataBegin = 1;
i32 const constexpr dataEnd = 51;
int main() {
    std::filesystem::path cur_path{"."};
    for (i32 i = dataBegin; i < dataEnd; i++) {
        auto input_path = cur_path / dirName / std::format(inputFormat, i);
        auto output_path = cur_path / dirName / std::format(outputFormat, i);

        if (not std::filesystem::exists(input_path.parent_path())) {
            std::filesystem::create_directories(input_path.parent_path());
        }
        std::fstream to_in(input_path, std::ios::out | std::ios::binary);
        Generator::generate(to_in);
        to_in.close();

        std::string call = std::format("{} < {} > {}", standardProgram, input_path.string(), output_path.string());
        std::cout << call << std::endl;
        std::system(call.c_str());

        std::fstream read_out(output_path, std::ios::in | std::ios::binary);
        std::string s;  char ch;
        while (read_out.get(ch))  s.push_back(ch);

        read_out.close();
        std::fstream write_out(output_path, std::ios::out | std::ios::binary);
        for (auto ch: s) {
            if (ch != '\r')  write_out << ch;
        }
        write_out.close();
    }
}