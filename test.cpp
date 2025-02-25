#include <bits/stdc++.h>
#ifdef _WIN32 
#include <windows.h>
#else  // not def _WIN32
#include <unistd.h>
#include <sys/wait.h>
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

int main() {
}
