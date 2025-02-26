#include <map>
#include <ranges>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>
#include <cassert>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace views = std::views;
namespace ranges = std::ranges;

#include "./libs/fixed_int.hpp"

char const constexpr endl = '\n';

char const constexpr *defaultArgumentsFileName = ".runcpp_default_args";
char const constexpr *compileArgumentsPresetsFileName = ".runcpp_presets";
char const constexpr *configFileName = ".runcpp_config";
char const constexpr *prevTimeFileName = ".runcpp_prev_time";
char const constexpr *noInputFileMessage = "No input file.";
char const constexpr *chooseMatchingFileMessage = "Please choose from following matched files (-1 for none):";
char const constexpr *templateFileName = "template.cpp";
char const constexpr *openWithEditorCommand = "code {}";
char const constexpr *fileNotFoundMessage = "File Not Found";
char const constexpr *compileCommand = "g++ {0}.cpp -o {0}.exe {1}";
char const constexpr *compileErrorMessage = "Compile Error occurred";
char const constexpr *compilingMessage = "Compiling...";
char const constexpr *runMessage = "========= RUN =========";
char const constexpr *runDebugMessage = "======== DEBUG ========";
char const constexpr *runCommand = "{}.exe";
char const constexpr *runDebugCommand = "{}.exe -d";
char const constexpr *runGDBCommand = "gdb {}.exe";
char const constexpr *exitMessage = "Exited with code {} in {:.4} seconds";
char const constexpr *setPresetMessage = "Set preset \"{}\" to \"{}\"? (y/n)";
char const constexpr *removePresetMessage = "Remove preset \"{}\"? (y/n)";
char const constexpr *invalidPresetNameMessage = "Preset name \"{}\" is invalid";
char const constexpr *skipCompile = "Skipped compiling \"{}\"";
int constexpr noInputFileCode = 1;
int constexpr fileNotFoundCode = 2;
int constexpr compileErrorCode = 3;
int constexpr runtimeErrorCode = 4;
bool constexpr openWithEditorOnCreate = true;

auto getConfigPath() -> std::string;

#ifdef _WIN32  // Windows
auto getConfigPath() -> std::string {
    static char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, path))) {
        return std::string(path) + "\\runcpp\\";
    }
    return "";
}
#else  // Mac/Linux
auto getConfigPath() -> std::string {
    char const *home = std::getenv("HOME");
    if (home != nullptr) {
        std::filesystem::path config_dir{home};
        config_dir /= ".config";
        return config_dir.string() + "/runcpp/";
    }
    return "";
}

#endif  // def _WIN32

/**
 * 启动一个程序并等待
 * @param program 程序名
 * @param args 参数列表
 * @returns 程序返回值
 */
auto launch(std::string const &program, std::string const &args) -> int {
#ifdef _WIN32
    STARTUPINFOA si;  // 启动信息
    PROCESS_INFORMATION pi = {};  // 进程信息

    // 初始化 si
    std::memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    std::signal(SIGINT, SIG_IGN);
    SetConsoleCtrlHandler(nullptr, TRUE);  // 禁用控制台事件处理
    std::string cmd = program + " " + args;
    auto success = CreateProcessA(
        nullptr,  // 应用程序名称（从命令解析）
        cmd.data(),  // 命令行
        nullptr,
        nullptr,
        FALSE,  // 继承句柄
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (not success) {
        SetConsoleCtrlHandler(nullptr, FALSE);  // 恢复控制台事件处理
        return -1;
    }

    CloseHandle(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);  // 等待子进程结束

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);  // 关闭进程
    
    SetConsoleCtrlHandler(nullptr, FALSE);  // 恢复控制台事件处理
    std::signal(SIGINT, SIG_DFL);
    return static_cast<int>(exitCode);

#else  // not def _WIN32
    pid_t pid = fork();
    if (pid < 0) {  // 失败
        return -1;
    } else if (pid == 0) {
        // 作为子进程
        std::signal(SIGINT, SIG_DFL);
        // 手动解析参数
        auto it = args.begin();
        std::vector<std::string> parts {program};
        std::vector<char *> pointers;
        while (it != args.end()) {
            auto find = std::find(it, args.end(), ' ');
            parts.push_back(std::string(it, find));
            if (find != args.end())  find++;
            it = find;
        }
        for (auto &part: parts) {
            pointers.push_back(part.data());
        }
        pointers.push_back(nullptr);
        execvp(program.c_str(), pointers.data());
        return -1;
    } else {
        // 作为父进程，忽略 SIGINT 中断
        std::signal(SIGINT, SIG_IGN);

        int status;
        waitpid(pid, &status, 0);

        std::signal(SIGINT, SIG_DFL);  // 恢复

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            std::cerr << 111 << std::endl;
            return -1;
        }
        return -1;
    }
#endif  // def _WIN32
    return -1;
}
/**
 * 启动一个程序
 * 
 * @param command 完整的命令，相当于空格分隔的 program 和 args
 */
auto launch(std::string const &command) -> int {
    auto space = command.find(' ');
    if (space == std::string::npos)  return launch(command, "");
    else  return launch(command.substr(0, space), command.substr(space + 1));
}

auto replace_string(std::string const &base, std::string const &match, std::string const &replace) -> std::string {
    std::string res;
    auto it = base.begin();
    std::boyer_moore_searcher searcher{match.begin(), match.end()};
    while (it != base.end()) {
        auto find = std::search(it, base.end(), searcher);
        ranges::copy(it, find, std::back_inserter(res));
        it = find;
        if (find != base.end()) {
            res += replace;
            it += match.size();
        }
    }
    return res;
}
auto replace_string_generate(std::string const &base, std::string const &match, auto &&f) -> std::string {
    std::string res;
    auto it = base.begin();
    std::boyer_moore_searcher searcher{match.begin(), match.end()};
    while (it != base.end()) {
        auto find = std::search(it, base.end(), searcher);
        ranges::copy(it, find, std::back_inserter(res));
        it = find;
        if (find != base.end()) {
            res += f();
            it += match.size();
        }
    }
    return res;
}

auto get_file_content(std::string const &filename) -> std::string {
    std::fstream fin(filename, std::ios::in);
    std::string content, line;
    while (std::getline(fin, line))  content += line, content += '\n';
    return content;
}

auto set_file_content(std::string const &filename, std::string const content) -> void {
    std::fstream fout(filename, std::ios::out);
    assert(fout);
    fout << content;
}

using Time = std::time_t;
auto getFileLastModifyTime(std::string const &path) -> Time {
    auto lastTime = std::filesystem::last_write_time(path).time_since_epoch();

    lastTime -= std::filesystem::file_time_type::clock::now().time_since_epoch();
    lastTime += std::chrono::system_clock::now().time_since_epoch();
    // std::cout << "* " << path << " " << lastTime << std::endl;
    return std::chrono::duration_cast<std::chrono::seconds>(lastTime).count();
}

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
    auto hasKey(std::string const &key) -> bool {
        return items.contains(key);
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
        auto dir = std::filesystem::path{filename}.parent_path();
        if (not std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);  // 不存在就创建
        }
        std::fstream fout(filename, std::ios::out);
        if (not fout.is_open())  assert(false);
        for (auto const &[key, value]: items) {
            if (value.empty())  continue;
            fout << key << ":" << value << endl;
        }
    }
    auto getItemsMap() -> std::unordered_map<std::string, std::string> const & {
        return items;
    }
};

class ArgumentManager {
    std::unordered_map<std::string, std::string> map;
    std::vector<std::string> values;
public:
    ArgumentManager(int argc, char const *argv[], Configure *default_config) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; i++) {
            args.emplace_back(argv[i]);
        }

        if (default_config != nullptr) {
            for (auto const &[key, value]: default_config->getItemsMap()) {
                if (value == "0")  continue;
                map.insert({key, value});
            }
        }

        for (auto it = args.begin(); it != args.end(); it++) {
            auto const &x = *it;

            if (x.starts_with("-")) {
                auto name = x.substr(1);
                auto set_default = false;
                std::string value = "1";
                if (name.starts_with("-")) {
                    name = name.substr(1);
                    set_default = true;
                }
                if (name.find('=') != std::string::npos) {
                    auto pos = name.find('=');
                    value = name.substr(pos + 1);
                    name = name.substr(0, pos);
                }
                if (name.starts_with("no-")) {
                    name = name.substr(3);
                    value = "";
                }

                if (set_default and default_config != nullptr) {
                    default_config->setItem(name, value);
                }
                map[name] = value;
                if (set_default and default_config != nullptr) {
                    default_config->setItem(name, value);
                }
            } else {
                values.push_back(x);
            }
        }

        if (default_config != nullptr) {
            default_config->save();
        }
    }
    auto has(std::string const &key) -> bool {
        auto it = map.find(key);
        return it != map.end() and not it->second.empty();
    }
    auto getOptionsMap() -> std::unordered_map<std::string, std::string> const & {
        return map;
    }
    auto getValues() -> std::vector<std::string> const & {
        return values;
    }
};

auto showHelp() -> void {
    std::cout << R"(用法 runcpp.exe [options]
--X 表示将 -X 设为默认值。
-no-X 表示禁用 X。
-create 自动新建不存在的文件。
-expand-include 展开使用双引号的 #include 语句。
-help 显示帮助。
-preset=<name> 使用预设的编译选项。
-search 尝试模糊匹配已经存在的文件。

-config-preset 进入预设配置模式
-config-preset <name> <content> 添加或修改一个预设编译选项
-config-preset <name> 删除一个预设
-condig-preset list 列出所有预设
)" << std::endl;
}

auto processFileName(std::string filename) -> std::string {
    if (filename.starts_with("./") or filename.starts_with(".\\")) {
        filename = filename.substr(2);
    }
    
    std::string symbols = " /\\:*?\"<>|-,';";  // 将要被替换为下划线的符号
    for (auto ch: symbols)  filename = replace_string(filename, {ch}, "_");
    
    if (not filename.ends_with(".cpp")) {
        filename += ".cpp";
    }
    return filename;
}

// 尝试从当前目录的文件中匹配目标文件
auto matchFileName(std::string const &x) -> std::string {
    auto match = x.ends_with(".cpp")? x.substr(0, x.size() - 4): x;
    std::filesystem::path cur_dir{"."}; 
    auto dir_it = std::filesystem::directory_iterator{cur_dir};

    std::vector<std::string> files;
    for (auto const &entry: dir_it) {
        auto path = entry.path();
        auto str = path.filename().string();
        if (str.ends_with(".cpp")) {
            auto processed = processFileName(str);
            files.push_back(processed.substr(0, processed.size() - 4));
        }
    }

    std::vector<std::string> options;
    [&]() -> void {
        std::vector<std::string> other;
        i32 constexpr maxCount = 10;
        for (auto const &x: files) {
            if (x.starts_with(match)) {
                options.push_back(x);

                if (static_cast<i32>(options.size()) == maxCount)  return;
            }
        }

        for (auto const &x: files) {
            if (not x.starts_with(match)) {
                if (x.find(match) != std::string::npos) {
                    options.push_back(x);
                    
                    if (static_cast<i32>(options.size()) == maxCount)  return;
                } else {
                    other.push_back(x);
                }
            }
        }

        auto similar = [&](std::string s) -> i32 {
            i32 res = 0;
            for (auto ch: match) {
                if (s.find(ch) != std::string::npos) {
                    res++;
                }
            }
            return res;
        };

        auto remains = std::min<i32>(maxCount - options.size(), other.size());
        ranges::partial_sort(other, other.begin() + remains, std::greater{}, similar);

        for (i32 j = 0; j < remains; j++) {
            options.push_back(other[j]);
        }
    }();

    std::cout << chooseMatchingFileMessage << std::endl;
    for (i32 i = 0; i < static_cast<i32>(options.size()); i++) {
        std::cout << i << ") " << options[i] << std::endl;
    }

    i32 choice;  std::cin >> choice;
    if (0 <= choice and choice < static_cast<i32>(options.size())) {
        return processFileName(options[choice]);
    } else {
        return "";
    }
}

int main(int argc, char const *argv[]) {
    Configure default_args{getConfigPath() + defaultArgumentsFileName};
    Configure compile_presets{getConfigPath() + compileArgumentsPresetsFileName};
    Configure prev_time{getConfigPath() + prevTimeFileName};  // 文件上次编译时间
    Configure config{getConfigPath() + configFileName};  // 其他配置项
    ArgumentManager am{argc, argv, &default_args};

    if (am.has("help")) {
        showHelp();
    } else if (am.has("config-preset")) {
        std::vector<std::string> values;
        for (int i = 1; i < argc; i++) {
            if (std::strcmp(argv[i], "-config-preset") == 0)  continue;
            values.push_back(argv[i]);
        }
        if (values.size() >= 1) {
            auto index = values[0];
            std::vector<std::string> reserved{"list"};

            auto presets = compile_presets.getItemsMap();
            if (values.size() == 1) {
                if (index == "list") {
                    for (auto const &[key, val]: presets) {
                        std::cout << key << ": " << val << std::endl;
                    }
                } else {
                    std::cout << std::format(removePresetMessage, index) << std::endl;
                    char choice;  std::cin >> choice;
                    if (choice == 'y' or choice == 'Y') {
                        if (presets.contains(index)) {
                            compile_presets.setItem(index, "");
                        }
                    }
                }
            } else {
                if (ranges::contains(reserved, index)) {
                    std::cout << std::format(invalidPresetNameMessage, index) << std::endl;
                    return 0;
                }
                auto to = values[1];
                std::cout << std::format(setPresetMessage, index, to) << std::endl;
                char choice;  std::cin >> choice;
                if (choice == 'y' or choice == 'Y') {
                    compile_presets.setItem(index, to);
                }
            }
        }
    } else {
        if (am.getValues().size() < 1) {
            std::cerr << noInputFileMessage << std::endl;
            return noInputFileCode;
        }
        
        auto filename = am.getValues().front();
        filename = processFileName(filename);
        auto original_filename = filename;  // 用户输入的文件名

        if (not std::filesystem::exists(filename)) {
            if (am.has("search")) {  // 搜索已有文件
                filename = matchFileName(filename);
            } else {
                filename = "";
            }
        }
        if (filename.empty()) {
            if (am.has("create")) {  // 创建文件
                std::filesystem::path template_path{templateFileName};
                if (std::filesystem::exists(template_path)) {
                    std::filesystem::copy(template_path, original_filename);
                } else {
                    std::fstream fs(original_filename, std::ios::out);
                    if (fs)  fs.close();
                    else  assert(false);
                }
                auto content = get_file_content(original_filename);
                auto hashName = [&]() -> std::string {
                    u64 hash = std::hash<std::string>{}(original_filename);
                    hash = hash % 9000'0000'0000'0000LL + 1000'0000'0000'0000LL;
                    return std::to_string(hash);
                }();
                content = replace_string(content, "/*hashName*/", hashName);
                set_file_content(original_filename, content);
                if (openWithEditorOnCreate) {
                    std::system(std::format(openWithEditorCommand, original_filename).c_str());
                }
                return 0;
            } else {
                std::cerr << fileNotFoundMessage << std::endl;
                return fileNotFoundCode;
            }
        }

        if (am.has("expand-include")) {  // 展开带有双引号的头文件
            std::fstream fin(filename, std::ios::in);
            std::string s, line;
            while (std::getline(fin, line))  s += line, s += "\n";
            fin.close();

            if (not s.contains("Do not expand include")) {
                std::stringstream ss(s);
                std::string line;
                std::string res;
                while (std::getline(ss, line)) {
                    if (line.starts_with("#include")) {
                        // 查找两个引号
                        auto it1 = ranges::find(line.begin(), line.end(), '\"');
                        auto it2 = ranges::find(std::next(it1), line.end(), '\"');
                        if (it1 != line.end() and it2 != line.end()) {  // 这之间的范围是文件名
                            std::string filename;
                            ranges::copy(std::next(it1), it2, std::back_inserter(filename));

                            std::fstream source_file(filename, std::ios::in);
                            if (source_file) {
                                std::string file_content, line;
                                while (std::getline(source_file, line))  file_content += line, file_content += '\n';
                                res += file_content;
                                continue;
                            }
                        }
                    }
                    res += line, res += '\n';
                }

                std::fstream fout(filename, std::ios::out);
                fout << res;
            }
        }

        auto filename_no_ext = filename.substr(0, filename.size() - 4);  // 无扩展名
        // 获取编译选项
        std::string compile_args;
        if (am.has("preset")) {
            auto index = am.getOptionsMap().at("preset");
            auto it = compile_presets.getItemsMap().find(index);
            if (it != compile_presets.getItemsMap().end()) {
                compile_args = it->second;
            }
        }
        // 检查能不能省略编译
        auto hashed_compile_args = std::hash<std::string>{}(compile_args);
        auto ellipsis = [&]() -> bool {
            if (config.hasKey("lastCompileArgs")) {
                if (config.getItem<uz>("lastCompileArgs") == hashed_compile_args) {
                    // 检查文件的修改时间
                    auto isFileModified = [&](std::string const &path) -> bool {
                        Time constexpr eps = 2;  // 差值在此范围内认为没被修改
                        try {
                            auto record_time = prev_time.getItem<Time>(path);
                            auto real_time = getFileLastModifyTime(path);
                    
                            return record_time - real_time < -eps;
                        } catch (Configure::KeyError &) {
                            return true;
                        }
                    };
                    return not isFileModified(filename);
                }
            }
            return false;
        }();

        if (ellipsis) {
            std::cout << std::format(skipCompile, filename) << std::endl;
        } else {
            auto compile_command = std::format(compileCommand, filename_no_ext, compile_args);
            std::cout << "> " << compile_command << std::endl;
            std::cout << compilingMessage << std::flush;
            auto compiler_code = launch(compile_command.c_str());

            if (compiler_code != 0) {
                std::cerr << std::endl;
                std::cerr << compileErrorMessage << std::endl;
                return compileErrorCode;
            }

            prev_time.setItem(filename, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            config.setItem("lastCompileArgs", std::hash<std::string>{}(compile_args));
            std::cout << "\r";
        }
        
        if (am.has("gdb")) {
            launch(std::format(runGDBCommand, filename_no_ext).c_str());
            return 0;
        }
        if (am.has("debug")) {
            std::cout << runDebugMessage << std::endl;
        } else {
            std::cout << runMessage << std::endl;
        }
        auto begin = std::chrono::system_clock::now();
        auto runtime_code = launch(
            am.has("debug")
                ? std::format(runDebugCommand, filename_no_ext).c_str()
                : std::format(runCommand, filename_no_ext).c_str()
        );
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::format(exitMessage, runtime_code & 255, static_cast<double>(duration.count()) / 1e9) << std::endl;
        if (runtime_code != 0)  return runtimeErrorCode;
        else  return 0;
    }
    return 0;
}