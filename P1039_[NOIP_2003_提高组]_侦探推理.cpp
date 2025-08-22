/**
 * @link https://www.luogu.com.cn/problem/P1039
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
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
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4084982378908579 {
namespace {
    std::unordered_set<std::string> all_names;

    using namespace std::string_literals;

    std::set<std::string> weekdays{
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    };

    struct Sentence {
        struct None {};
        // 判断某人是/否为罪犯
        struct Guilty {
            std::string name{};
            bool word{};
        };
        struct WeekDay {
            std::string word;
        };

        std::variant<None, Guilty, WeekDay> data;
        template <typename T>
        auto get_if() -> T * {
            return std::get_if<T>(&data);
        }
        template <typename T>
        auto get_if() const -> T const * {
            return std::get_if<T>(&data);
        }

        auto static parse(std::string line) -> std::pair<std::string, Sentence> {
            if (line.empty()) return {};
            while (line.back() <= ' ') line.pop_back();

            auto pos = line.find(':');
            if (pos == std::string::npos) return {};
            if (line[pos + 1] != ' ') return {};

            auto name = line.substr(0, pos);
            auto info = line.substr(pos + 2);

            if (!all_names.contains(name)) return {};

            auto someone_guilty = [&](bool word) -> Sentence {
                std::array<uz, 2> static lens{
                    " is not guilty."s.size(),
                    " is guilty."s.size(),
                };

                auto obj_name = info.substr(0, info.size() - lens[word]);
                if (!all_names.contains(obj_name)) return {};
                else return {Guilty{.name = obj_name, .word = word}};
            };

            if (info == "I am guilty.") return {name, {Guilty{name, true}}};
            else if (info == "I am not guilty.") return {name, {Guilty{name, false}}};
            else if (info.ends_with(" is guilty.")) {
                return {name, someone_guilty(true)};
            } else if (info.ends_with(" is not guilty.")) {
                return {name, someone_guilty(false)};
            } else if (info.starts_with("Today is ")) {
                auto static len = "Today is "s.size();
                auto day = info.substr(len);
                day.pop_back(); // 句号

                if (!weekdays.contains(day)) return {};
                return {name, {WeekDay{day}}};
            } else {
                return {};
            }
        }
    };
    auto solve() -> void {
        i32 m{}, n{}, p{};
        std::cin >> m >> n >> p;

        for (auto _ = m; _ --> 0; ) {
            std::string s; std::cin >> s;
            all_names.emplace(std::move(s));
        }

        char ch{};
        for (; ch != '\n'; std::cin.get(ch));

        std::unordered_map<std::string, std::vector<Sentence>> st;
        for (auto _ = p; _ --> 0; ) {
            std::string line;
            std::getline(std::cin, line);
            auto [name, info] = Sentence::parse(line);
            if (info.get_if<Sentence::None>() == nullptr) {
                st[name].emplace_back(info);
            }
        }

        // 枚举罪犯
        std::unordered_set<std::string> possible;
        for (auto const &person: all_names) {
            for (auto const &day: weekdays) {
                if (person == "MIKE" && day == "Sunday") {
                    auto _ = 0;
                }
                enum class Tag: std::uint8_t {
                    Unknown, Fake, Real
                };
                std::unordered_map<std::string, Tag> tags;
                bool good = true;

                auto assign = [&](auto const &key, auto const &val) -> void {
                    if (tags[key] != Tag::Unknown && tags[key] != val) good = false;
                    tags[key] = val;
                };
                for (auto const &[cur, info_vec]: st) {
                    for (auto const &info: info_vec) {
                        if (auto *p = info.get_if<Sentence::Guilty>()) {
                            auto const &[target, word] = *p;
                            if ((target == person) == word) assign(cur, Tag::Real);
                            else assign(cur, Tag::Fake);
                        } else if (auto *p = info.get_if<Sentence::WeekDay>()) {
                            if (day == p->word) assign(cur, Tag::Real);
                            else assign(cur, Tag::Fake);
                        }
                        if (!good) break;
                    }
                    if (!good) break;
                }
                if (!good) continue;

                auto fake = static_cast<i32>(ranges::count(tags | views::values, Tag::Fake));
                auto unknown = static_cast<i32>(m - tags.size());

                if (fake <= n && fake + unknown >= n) {
                    possible.emplace(person);
                }
            }
        }

        if (possible.empty()) std::cout << "Impossible\n";
        else if (possible.size() > 1) std::cout << "Cannot Determine\n";
        else {
            auto name = *possible.begin();
            std::cout << name << '\n';
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4084982378908579::solve();
    return 0;
}
