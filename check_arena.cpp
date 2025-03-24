// Do not expand include
#include "lib"
#include "libs/range.hpp"
#include "libs/fixed_int.hpp"
#ifdef __linux__
#define linux
#endif  // def __linux__
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
#ifdef linux
constexpr const char *CompileCommand = "g++ {} -o {}.exe -O2 -std=c++23 -Wall -Wextra -fsanitize=address,undefined -DONLINE_JUDGE=LUOGU_GZA";
#else  // not def linux
constexpr const char *CompileCommand = "g++ {} -o {}.exe -O2 -std=c++23 -Wall -Wextra -Wl,-stack=2147483647 -DONLINE_JUDGE=LUOGU_GZA";
#endif  // def linux

constexpr const char *PrevTimeFilename = ".checkarena_prev_time";  // 存储上次修改时间的配置文件名

constexpr const bool InterruptOnConflict = true;       // 结果出现冲突时中断进程
constexpr const bool InterruptOnMainConflict = true;   // 特别地，当 file[0] 结果出现冲突，中断进程
constexpr const bool ClearEachTime = false;

#ifdef linux    
constexpr const char *RunCommand = "./{}.exe < {} > {}_{}";
#else  // not def linux
constexpr const char *RunCommand = "{}.exe < {} > {}_{}";
#endif  // def linux

namespace _Generator {
#include "check_gen.cpp"
}
namespace Generator = _Generator::Generator;

namespace Checker {
    class Configure {
        using Self = Configure;
        std::string filename;
        std::unordered_map<std::string, std::string> items;
        
        public:
        struct ParseError: public std::exception {
            std::string message;
            ParseError(std::string const &s): message(s) {}
            
            auto what() const noexcept -> char const * override {
                return message.c_str();
            }
        };
        struct KeyError: public std::exception {
            std::string message;
            KeyError(std::string const &s): message(s) {}
            
            auto what() const noexcept -> char const * override {
                return message.c_str();
            }
        };
        Configure(std::string const &filename): filename(filename) {
            std::fstream fin(filename, std::ios::in);
            if (not fin.good())  return;
            std::string line;
            while (std::getline(fin, line)) {
                auto split = line.find(':');
                if (split == std::string::npos)  throw ParseError{"Cannot get ':'."};
                items.insert({line.substr(0, split), line.substr(split + 1)});
            }
        }
        ~Configure() {
            save();
        }

        template <typename T>
        auto getItem(std::string const &key, T &&val) -> T {
            if (not items.contains(key))  return val;
            std::stringstream st(items.at(key), std::ios::in);

            T x;  st >> x;
            return x;
        }
        template <typename T>
        auto getItem(std::string const &key) -> T {
            if (not items.contains(key))  throw KeyError{"Unknown Key: " + key};
            std::stringstream st(items.at(key), std::ios::in);

            T x;  st >> x;
            return x;
        }
        template <typename T>
        auto setItem(std::string const &key, T &&val) -> void {
            std::stringstream st;

            st << std::forward<T>(val);
            items[key] = st.str();
        }
        auto save() -> void {
            std::fstream fout(filename, std::ios::out);
            if (not fout.is_open())  assert(false);
            for (auto const &[key, value]: items) {
                fout << key << ": " << value << endl;
            }
        }
    };
    Configure prevTimeConfig{PrevTimeFilename};

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

    using Time = std::time_t;
    auto getFileLastModifyTime(std::string const &path) -> Time {
        auto lastTime = std::filesystem::last_write_time(path).time_since_epoch();

        lastTime -= std::filesystem::file_time_type::clock::now().time_since_epoch();
        lastTime += std::chrono::system_clock::now().time_since_epoch();
        // std::cout << "* " << path << " " << lastTime << std::endl;
        return std::chrono::duration_cast<std::chrono::seconds>(lastTime).count();
    }
    auto isFileModified(std::string const &path) -> bool {
        Time constexpr eps = 2;  // 差值在此范围内认为没被修改
        try {
            auto record_time = prevTimeConfig.getItem<Time>(path);
            auto real_time = getFileLastModifyTime(path);
    
            return record_time - real_time < -eps;
        } catch (Configure::KeyError &) {
            return true;
        }
    }
    char tmp[1024];
    void compile() {
        for (auto i: range(FileCount)) {
            if (isFileModified(FileNames[i])) {
                *std::format_to(tmp, CompileCommand, FileNames[i], i) = 0;
                std::cout << tmp << std::endl;
                if (std::system(tmp))  throw std::string(FileNames[i]) + "compile error";
                prevTimeConfig.setItem(FileNames[i], std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        } else {
                std::cout << std::format("File \"{}\" skipped compile!", FileNames[i]) << std::endl;
            }
        }
        prevTimeConfig.save();
    }

    void clear() {
#ifndef linux
        std::system("cls");
#else
        std::system("clear");
#endif
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
                *std::format_to(tmp, RunCommand, i, InputFileName, OutputFileName, i) = 0;
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

            auto some_runtime_error = false;
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
                    if (ret)                    std::cout << std::format("#{} Runtime Error {:.2f}sec", i, dur), some_runtime_error = true;
                    else {
                        if (ans == max_hash)    std::cout << std::format("#{} Accepted      {:.2f}sec", i, dur);
                        else                    std::cout << std::format("#{} Wrong Answer  {:.2f}sec", i, dur);
                    }
                    if (i == best.second)  std::cout << " Best Solution!";
                    std::cout << std::endl;
                }

                if (InterruptOnMainConflict and (max_hash != results.at(0).ansHash or results.at(0).ret != 0))  pause();
            } else {
                std::cout << std::format("Cannot infer answer on test {}", i) << std::endl;
            }
            if (InterruptOnConflict and (max_count.first != FileCount or some_runtime_error))  pause();

            {
                using namespace std::chrono_literals;
                // std::this_thread::sleep_for(100ms);
            }
        }
    }
}

int main() {
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
