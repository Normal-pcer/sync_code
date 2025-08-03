[[noreturn]] auto panic_impl(std::string_view msg, char const *file, unsigned int line, char const *func) -> void {
    std::cerr << std::format("Panicked at {}:{}, in function {}", file, line, func) << '\n';
    std::cerr << msg << '\n';
    std::abort();
}
#define PANIC(msg) panic_impl(msg, __FILE__, __LINE__, __PRETTY_FUNCTION__), __builtin_unreachable()
