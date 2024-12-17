// Do not expand include
#include "./lib_v2.hpp"
#include "./libs/range.hpp"

using namespace lib;

constexpr const int MaxTimes = inf;
constexpr const int FileCount = 2;
constexpr const char *FileNames[] = {
    "check_arena0.cpp",
    "check_arena1.cpp",
    "check_arena2.cpp",
    "check_arena3.cpp",
    "check_arena4.cpp",
    "check_arena5.cpp",
};
constexpr const char *InputFileName = "1" ".in";
constexpr const char *OutputFileName = "1" ".out";
constexpr const char *CompileCommand = "g++ {} -o {} -O2 -std=c++23 -Wall -Wl,-stack=2147483647 -Wextra";
constexpr const bool InterruptOnConflict = true;       // 结果出现冲突时中断进程
constexpr const bool InterruptOnMainConflict = true;   // 特别地，当 file[0] 结果出现冲突，中断进程
constexpr const bool ClearEachTime = false;

namespace _Generator {
    #include "check_gen.cpp"
}
namespace Generator = _Generator::Generator;

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
        auto i = vec.end() - 1;
        for (; i->empty(); i--);
        vec.erase(i+1, vec.end());
    }

    size_t hash(std::vector<std::string> &vec) {
        ranges::for_each(vec, process);
        auto view_hash = vec | views::transform(lam(x, std::hash<std::string>{}(x)));
        size_t res = std::accumulate(view_hash.begin(), view_hash.end(), (size_t)0);
        return res;
    }

    char tmp[1024];
    void compile() {
        for (auto i: range(FileCount)) {
            *std::format_to(tmp, CompileCommand, FileNames[i], i) = 0;
            std::cout << tmp << std::endl;
            if (std::system(tmp))  throw std::string(FileNames[i]) + "compile error";
        }
    }

    void clear() {
        std::system("cls");
    }

    struct Result {
        size_t ansHash;
        double duration;
        int ret;
    };

    void main() {
        compile();

        for (auto i = 0; i != MaxTimes; i++) {
            if (ClearEachTime)  clear();
            std::cout << std::format("Running on test {}", i) << std::endl;
            std::fstream in(InputFileName, std::ios::out | std::ios::trunc);
            if (not in.is_open())  throw "failed to open input";
            generate(in);
            in.close();

            std::string line;
            std::vector<std::string> lines1, lines2;

            std::vector<Result> results;
            std::map<size_t, int> count;
            std::pair<int, size_t> max_count;

            for (auto i: range(FileCount)) {
                *std::format_to(tmp, "{}.exe < {} > {}_{}", i, InputFileName, OutputFileName, i) = 0;
                std::cout << std::format("Waiting #{}...\r", i) << std::flush;
                auto begin = std::chrono::system_clock::now();
                int ret = std::system(tmp);
                auto end = std::chrono::system_clock::now();

                auto duration = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000;

                std::fstream f(std::format("{}_{}", OutputFileName, i), std::ios::in);
                std::vector<std::string> this_result;
                std::string line;
                while (std::getline(f, line))  this_result.push_back(line);
                auto hashed = hash(this_result);

                results.push_back({.ansHash = hashed, .duration = duration, .ret = ret});
                count[hashed]++, chkMax(max_count, std::pair(count[hashed], hashed));
            }

            auto half = (FileCount + 1) >> 1;
            if (max_count.first >= half) {
                auto [max, max_hash] = max_count;
                std::pair best {1e300, -1};
                for (auto i: range(FileCount)) {
                    auto [ans, dur, ret] = results.at(i);
                    if (ans == max_hash and ret == 0)  chkMin(best, {dur, i});
                }

                for (auto i: range(FileCount)) {
                    auto [ans, dur, ret] = results.at(i);
                    if (ret)                    std::cout << std::format("#{} Runtime Error {:.2f}sec", i, dur);
                    else {
                        if (ans == max_hash)    std::cout << std::format("#{} Accepted      {:.2f}sec", i, dur);
                        else                    std::cout << std::format("#{} Wrong Answer  {:.2f}sec", i, dur);
                    }
                    if (i == best.second)  std::cout << " Best Solution!";
                    std::cout << std::endl;
                }

                if (InterruptOnMainConflict and max_hash != results.at(0).ansHash)  pause();
            } else {
                std::cout << std::format("Cannot infer answer on test {}", i) << std::endl;
            }
            if (InterruptOnConflict and max_count.first != FileCount)  pause();

            {
                using namespace std::chrono_literals;
                // std::this_thread::sleep_for(100ms);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    try {
        Checker::main();
    } catch (std::string str) {
        std::cerr << "terminate called after throwing an instance of 'std::string': " << std::endl;
        std::cerr << str << std::endl;
    } catch (const char *str) {
        std::cerr << "terminate called after throwing an instance of 'const char*': " << std::endl;
        std::cerr << str << std::endl;
    }

    return 0;
}