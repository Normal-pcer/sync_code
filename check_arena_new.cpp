// Do not expand include
#include <bits/stdc++.h>

namespace Generator_ {
#define INCLUDING_GENERATOR
#include "check_gen.cpp"
#undef INCLUDING_GENERATOR
}
namespace Generator = Generator_::Generator;

char constexpr endl = '\n';
namespace ranges = std::ranges;
namespace views = std::views;

using i32 = std::int32_t;
using i64 = std::int64_t;
using f64 = double;
using Filename = std::filesystem::path;
using Clock = std::chrono::system_clock;
using Duration = Clock::duration;

i32 constexpr fileCount = 3;
std::array<std::string_view, 6> constexpr filenames{{
    "check_arena0.cpp",
    "check_arena1.cpp",
    "check_arena2.cpp",
    "check_arena3.cpp",
    "check_arena4.cpp",
    "check_arena5.cpp"
}};
i32 constexpr maxTimes = 1'000'000;

/*
配置文件相关工具
*/
class Configure {
    using Self = Configure;
    Filename filename;
    std::unordered_map<std::string, std::string> items;

public:
    struct ParseError: public std::exception {
        std::string message;
        ParseError(std::string s): message(std::move(s)) {}
        
        [[nodiscard]] auto what() const noexcept -> char const * override {
            return message.c_str();
        }
    };
    struct KeyError: public std::exception {
        std::string message;
        KeyError(std::string s): message(std::move(s)) {}
        
        [[nodiscard]] auto what() const noexcept -> char const * override {
            return message.c_str();
        }
    };
    explicit Configure(Filename filename_): filename(std::move(filename_)) {
        std::fstream fin(filename, std::ios::in);
        if (not fin.good()) {
            std::cerr << "Cannot open file: " << filename.string() << std::endl;
            return;
        }
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

struct ProgramOutput {
    i32         exitCode;
    Filename     filename;
    Duration     time;
};

// 检查器
class Judger {
public:
    struct Result {
        std::string_view     message;
        bool                 isAccepted;
        Duration             time;
    };

    auto virtual judge(std::vector<ProgramOutput> const &out) -> std::vector<Result> = 0;
};

// 比较两个输出
class Comparer {
public:
    auto virtual compare(Filename const &a, Filename const &b) -> bool = 0;
};

// 默认的比较器，去除文件结尾的空行、每行末尾的空格
class DefaultComparer: public Comparer {
    auto process(Filename const &file) const -> std::string {
        std::ifstream fin(file);
        std::vector<std::string> lines;

        for (std::string line; std::getline(fin, line); ) {
            while (not line.empty() and line.back() == ' ') line.pop_back();
            if (not line.empty()) lines.push_back(std::move(line));
        }

        std::string s;
        for (auto const &line: lines) s += line;
        return s;
    }
public:
    auto compare(Filename const &a, Filename const &b) -> bool override {
        return process(a) == process(b);
    }
};

// 并查集
class DSU {
    std::vector<i32> fa, size;
public:
    DSU(i32 n): fa(n + 1), size(n + 1, 1) {
        ranges::iota(fa, 0);
    }

    auto find(i32 x) -> i32 {
        if (x == fa[x]) return x;
        else return fa[x] = find(fa[x]);
    }

    auto merge(i32 x, i32 y) -> void {
        x = find(x), y = find(y);
        if (size[x] < size[y]) std::swap(x, y);
        fa[y] = x, size[x] += size[y];
    }
};

// 通过比较几个输出，来判断每个答案是否正确。
template <typename Comparer>
class CompareJudger: public Judger {
    Comparer cmp;
public:
    CompareJudger(Comparer cmp): cmp(std::move(cmp)) {}

    auto judge(std::vector<ProgramOutput> const &out) -> std::vector<Result> override { 
        auto n = static_cast<i32>(out.size());
        std::vector<Result> res(n);

        std::vector<i32> waiting;  // 等待结果
        DSU dsu{n};
        for (i32 i = 0; i < n; i++) {
            if (out[i].exitCode != 0) {
                res[i] = {
                    .message = "Runtime Error",
                    .isAccepted = false,
                    .time = out[i].time,
                };
                continue;
            }
            // 每一个文件，尝试和之前的联通块比较
            for (i32 j = 0; j < i; j++) {
                if (dsu.find(j) != j) continue;
                if (cmp.compare(out[j].filename, out[i].filename)) {
                    dsu.merge(j, i);
                }
            }
            waiting.push_back(i);
        }

        // 尝试判定答案
        // 提取每个联通块的所有文件
        std::vector<std::vector<i32>> blocks(n);
        for (auto i: waiting) {
            blocks[dsu.find(i)].push_back(i);
        }

        // 如果存在绝对众数，认为是正确答案
        auto isAnswer = [&](std::size_t cnt) -> bool { return cnt * 2 > waiting.size(); };
        i32 ans = -1;
        for (auto i: waiting) {
            if (isAnswer(blocks[i].size())) ans = i;
        }

        if (ans == -1) {
            // 无法判定答案，返回全 WA
            for (auto i: waiting) {
                res[i] = {
                    .message = "Wrong Answer",
                    .isAccepted = false,
                    .time = out[i].time,
                };
            }
        } else {
            for (auto i: waiting) {
                if (dsu.find(i) == dsu.find(ans)) {
                    res[i] = {
                        .message = "Accepted",
                        .isAccepted = true,
                        .time = out[i].time,
                    };
                } else {
                    res[i] = {
                        .message = "Wrong Answer",
                        .isAccepted = false,
                        .time = out[i].time,
                    };
                }
            }
        }
        return res;
    }
};

class FileUtils {
    std::string_view static constexpr configFilename = ".checkarena_prev_time";
    Configure static inline config{configFilename};
public:
    auto static isModified(Filename const &file) -> bool {
        using namespace std::chrono_literals;
        auto constexpr eps = 2s;
        try {
            using TimeNumber = i64;
            auto record = Clock::time_point(
                std::chrono::nanoseconds(config.getItem<TimeNumber>(file.string()))).time_since_epoch();
            auto realTime = std::filesystem::last_write_time(file).time_since_epoch()
                            - std::filesystem::file_time_type::clock::now().time_since_epoch();
                            + Clock::now().time_since_epoch();

            return record - realTime < -eps;
        } catch (Configure::KeyError &) {
            return true;
        }
    }
    auto static update(Filename const &file) -> void {
        config.setItem(file.string(), std::chrono::duration_cast<std::chrono::nanoseconds>(
            Clock::now().time_since_epoch()).count());
        config.save();
    }
};

class Compiler {
    std::string_view static constexpr format = "g++ {0}.cpp -o {0}.exe -std=c++23 -O2 -Wl,-stack=2147483647 -Wall -Wextra";
public:
    struct CompileError: std::exception {
        [[nodiscard]] auto what() const noexcept -> char const * override {
            return "A file has compile error.";
        }
    };
    auto static compile(Filename const &file) -> Filename {
        auto noExt = file.string();
        assert(noExt.ends_with(".cpp"));
        noExt = noExt.substr(0, noExt.size() - 4);

        Filename executable{noExt + ".exe"};
        if (FileUtils::isModified(file) or not std::filesystem::exists(executable)) {
            auto cmd = std::format(format, noExt);
            std::cout << cmd << std::endl;
            auto code = std::system(cmd.c_str());
            FileUtils::update(file);
            if (code != 0) throw CompileError{};
        }

        return executable;
    }
};

class Launcher {
    std::string_view static constexpr format = "{0} < {1} > {2}";
public:
    auto static launch(Filename const &exe, Filename const &input, Filename const &output) -> ProgramOutput {
        auto start = Clock::now();
        auto code = std::system(std::format(format, exe.string(), input.string(), output.string()).c_str());
        auto end = Clock::now();
        return {
            .exitCode = code,
            .filename = output,
            .time = end - start
        };
    }
};

auto getJudger() -> CompareJudger<DefaultComparer> {
    return {{}};
}

auto main() -> int {
    std::vector<Filename> executables;
    try {
        for (i32 i = 0; i < fileCount; i++) {
            executables.push_back(Compiler::compile(filenames[i]));
        }
    } catch (Compiler::CompileError &) {
        std::cerr << "Some files have compile error." << std::endl;
    }

    for (i32 i = 0; i < maxTimes; i++) {
        std::cout << "Running on test " << i << std::endl;

        {
            std::ofstream fout(Filename{"1.in"}, std::ios::out | std::ios::binary | std::ios::trunc);
            if (not fout.is_open()) throw "Failed to open 1.in.";

            Generator::generate(fout);
        }

        {
            std::vector<ProgramOutput> out;
            for (i32 i = 0; i < fileCount; i++) {
                std::cout << "Waiting #" << i << "...\r";
                out.push_back(Launcher::launch(executables[i], "1.in", std::format("1.out_{}", i)));
            }

            auto res = getJudger().judge(out);
            auto allAccepted = std::all_of(res.begin(), res.end(), [](auto const &r) { return r.isAccepted; });
            for (i32 i = 0; i < fileCount; i++) {
                std::cout << std::format(
                    "#{} {} {:.2f}s", i, res[i].message, static_cast<f64>(out[i].time.count()) * 1e-9) << std::endl;
            }

            if (not allAccepted) {
                std::cout << "Failed." << std::endl;
                std::string tmp;
                std::cin >> tmp;
                static_cast<void>(tmp);
            }
        }
    }

    return 0;
}