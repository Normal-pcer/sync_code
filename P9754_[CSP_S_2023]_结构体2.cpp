/**
 * @link https://www.luogu.com.cn/problem/P9754
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define _lambda_1(expr) [&]() { return expr; }
#define _lambda_2(a, expr) [&](auto a) { return expr; }
#define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
#define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
#include <unordered_map>

using namespace lib;

[[noreturn]] auto panic_impl(std::string_view msg, char const *file, unsigned int line, char const *func) -> void {
    std::cerr << std::format("Panicked at {}:{}, in function {}", file, line, func) << endl;
    std::cerr << msg << endl;
    std::abort();
}
#define PANIC(msg) panic_impl(msg, __FILE__, __LINE__, __PRETTY_FUNCTION__), __builtin_unreachable()

using namespace std::string_literals;

namespace Solution_4157617279219657 {
    using vsize_t = u64;  // 虚拟的“size_t”

    auto alignAddress(vsize_t addr, vsize_t align) -> vsize_t {
        assert(std::popcount(align) == 1);
        return (addr + align - 1) & ~(align - 1);  // 精妙的设计，避免除法开销
        // return (addr + align - 1) / align * align;
    }

    struct TypeInfo;

    struct Identifier {
        std::string name{};
        std::shared_ptr<TypeInfo> type{};
    };

    struct TypeInfo {
        std::string name{};
        vsize_t size{};
        vsize_t align{};

        // 成员列表
        std::map<vsize_t, Identifier> addrToField{};
        std::unordered_map<std::string, vsize_t> nameToAddr{};
        std::unordered_map<std::string, std::shared_ptr<TypeInfo>> nameToType{};

        TypeInfo(std::string name_, vsize_t size_)  // 内置类型
            : name(std::move(name_)), size(size_), align(size_) {}

        TypeInfo(std::string name_, 
                std::vector<Identifier> const &fields)
            : name(std::move(name_))
        {
            if (fields.empty()) {
                size = align = 1;
                return;
            }
            for (auto const &id: fields) {
                addField(id);
            }
        }

        // 仅应该在全局上调用
        auto addField(Identifier const &id) -> vsize_t {
            if (nameToAddr.contains(id.name)) {
                PANIC("Field already exists: " + id.name);
            }
            chkMax(align, id.type->align);

            vsize_t ptr = 0;
            if (not addrToField.empty()) {
                auto const &[last, lastId] = *addrToField.rbegin();
                ptr = alignAddress(last + lastId.type->size, id.type->align);
            }

            nameToAddr[id.name] = ptr;
            nameToType[id.name] = id.type;
            addrToField[ptr] = id;
            size = alignAddress(ptr + id.type->size, align);
            return ptr;
        }

        // 返回 .field1.subfield2 的形式，或者 nullopt
        auto accessPos(vsize_t ptr) const -> std::optional<std::string> {
            if (ptr >= size) return std::nullopt;
            if (addrToField.empty()) return "";
            auto fieldIt = addrToField.upper_bound(ptr);
            assert(fieldIt != addrToField.begin()), --fieldIt;  // 一定处于该成员内部/对齐空隙

            auto const &[begin, field] = *fieldIt;
            if (auto res = field.type->accessPos(ptr - begin); res.has_value()) {
                return "."s + field.name + *res;
            } else {
                return std::nullopt;
            }
        }

        auto access(std::string expr) -> vsize_t {
            auto dot = ranges::find(expr, '.');
            auto fieldName = dot == expr.end()
                ? expr
                : expr.substr(0, dot - expr.begin());
            auto it = nameToAddr.find(fieldName);

            if (it == nameToAddr.end()) {
                PANIC("Field not found: " + fieldName);
            }
            auto ans = it->second;
            if (dot != expr.end()) {
                ans += nameToType[fieldName]->access(expr.substr(dot - expr.begin() + 1));
            }
            return ans;
        }
    };

    std::unordered_map<std::string, std::shared_ptr<TypeInfo>> types{};
    
    TypeInfo global{"", {}};

    auto query() -> void {
        i32 op{}; std::cin >> op;

        if (op == 1) {
            std::string name; std::cin >> name;
            i32 cnt{}; std::cin >> cnt;

            std::vector<Identifier> fields;
            fields.reserve(cnt);

            for (i32 _ = 0; _ < cnt; ++_) {
                std::string type, field;
                std::cin >> type >> field;

                if (auto it = types.find(type); it != types.end()) {
                    auto typeInfo = it->second;
                    fields.emplace_back(field, typeInfo);
                } else {
                    PANIC("Type not found: " + type);
                }
            }

            auto typeInfo = std::make_shared<TypeInfo>(name, std::move(fields));
            types[name] = typeInfo;
            std::cout << typeInfo->size << ' ' << typeInfo->align << endl;
        } else if (op == 2) {
            // 定义变量
            std::string type, name;
            std::cin >> type >> name;

            auto it = types.find(type);
            if (it == types.end()) {
                PANIC("Type not found: " + type);
            }
            auto typeInfo = it->second;

            // 在当前的最后排布变量
            std::cout << global.addField({.name = name, .type = typeInfo}) << endl;
        } else if (op == 3) {
            // 访问表达式
            std::string expr;
            std::cin >> expr;

            auto pos = global.access(expr);
            std::cout << pos << '\n';
        } else {
            // 获取变量名
            vsize_t ptr{}; std::cin >> ptr;
            auto res = global.accessPos(ptr);
            if (res.has_value()) {
                // 跳过第一个点
                std::cout << res->substr(1) << endl;
            } else {
                std::cout << "ERR" << endl;
            }
        }
    }

    auto solve() -> void {
        i32 n; std::cin >> n;

        // 内置类型
        types["byte"] = std::make_shared<TypeInfo>("byte", 1);
        types["short"] = std::make_shared<TypeInfo>("short", 2);
        types["int"] = std::make_shared<TypeInfo>("int", 4);
        types["long"] = std::make_shared<TypeInfo>("long", 8);

        for (i32 _ = 0; _ < n; ++_) {
            query();
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4157617279219657::solve();
    return 0;
}
