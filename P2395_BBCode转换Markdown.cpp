/**
 * @link https://www.luogu.com.cn/problem/P2395
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

char const constexpr *matchError = "Match Error";
char const constexpr *unclosedMark = "Unclosed Mark";

namespace Solution_1783182971206534 {
    struct BBCLabel {
        enum Type { 
            h1, h2, i, b, url, img, quote, text, none
        } type = text;
        std::string value {};
        std::vector<BBCLabel *> children {};

        BBCLabel(Type t = text): type(t), value(), children() {}
        BBCLabel(BBCLabel const &) = delete;
        BBCLabel(BBCLabel &&) = default;
        ~BBCLabel() {
            for (auto ptr: children)  delete ptr;
        }
    };
    void solve() {
        struct EOFError {};
        auto gc = []() -> char {
            char ch;
            std::cin.get(ch);
            if (std::cin.eof())  throw EOFError{};
            return ch;
        };
        
        auto get_type = [&](std::string_view s) -> BBCLabel::Type {
#define JOIN_TYPE(type)  if (s == #type) { return BBCLabel::type; }
            JOIN_TYPE(h1) else
            JOIN_TYPE(h2) else
            JOIN_TYPE(b) else
            JOIN_TYPE(i) else
            JOIN_TYPE(url) else
            JOIN_TYPE(img) else
            JOIN_TYPE(quote) else
            assert(false), __builtin_unreachable();
        };

        char ch;
        std::vector<BBCLabel> st;
        st.emplace_back(BBCLabel::none);
        try {
            while (std::cin.get(ch)) {
                if (ch == '[') {
                    if ((ch = gc()) == '/') {
                        std::string op;
                        for (ch = gc(); ('a' <= ch and ch <= 'z') or ('0' <= ch and ch <= '9'); ch = gc())  op.push_back(ch);
                        auto type = get_type(op);
                        if (st.back().type == BBCLabel::text) {
                            auto tmp = new BBCLabel{std::move(st.back())};
                            st.pop_back();
                            st.back().children.push_back(tmp);
                        }
                        if (st.size() <= 1 or type != st.back().type or type == BBCLabel::quote) {
                            std::cout << matchError << std::endl;
                            return;
                        }
                        auto tmp = new BBCLabel{std::move(st.back())};
                        st.pop_back();
                        st.back().children.push_back(tmp);
                    } else {
                        std::string op;
                        for (; ('a' <= ch and ch <= 'z') or ('0' <= ch and ch <= '9'); ch = gc())  op.push_back(ch);
                        BBCLabel label;
                        
                        label.type = get_type(op);
                        if (ch == '=') {
                            std::string link;
                            ch = gc();
                            for (; ch != ']'; ch = gc())  link.push_back(ch);
                            label.value = std::move(link);
                        }
                        if (st.back().type == BBCLabel::text) {
                            auto tmp = new BBCLabel{std::move(st.back())};
                            st.pop_back();
                            st.back().children.push_back(tmp);
                        }
                        st.push_back(std::move(label));

                        if (get_type(op) == BBCLabel::quote) {
                            std::string content;
                            for (; not content.ends_with("[/quote]"); ) {
                                content.push_back(ch = gc());
                            }
                            for (i32 i = 0; i < 8; i++)  content.pop_back();
                            if (not content.empty() and content.back() == '\n')  content.pop_back();
                            st.back().value = std::move(content);
                            auto tmp = new BBCLabel{std::move(st.back())};
                            st.pop_back();
                            st.back().children.push_back(tmp);
                        }
                    }
                } else {
                    if (st.back().type == BBCLabel::text)  st.back().value.push_back(ch);
                    else  st.push_back({BBCLabel::text}), st.back().value.push_back(ch);
                }
            }
        } catch (EOFError &) {}
        if (st.back().type == BBCLabel::text) {
            auto tmp = new BBCLabel{std::move(st.back())};
            st.pop_back();
            st.back().children.push_back(tmp);
        }
        if (st.size() > 1) {
            std::cout << unclosedMark << std::endl;
            return;
        }

        std::string ans;
        auto dfs = [&](auto &&self, BBCLabel *ptr) -> void {
            if (ptr->type == BBCLabel::text)  ans += ptr->value;
            else {
                auto print_children = [&]() {
                    for (auto x: ptr->children) {
                        self(self, x);
                    }
                };
                if (ptr->type == BBCLabel::h1)  ans += "# ", print_children(), ans += " #";
                else if (ptr->type == BBCLabel::h2)  ans += "## ", print_children(), ans += " ##";
                else if (ptr->type == BBCLabel::i)  ans += "*", print_children(), ans += "*";
                else if (ptr->type == BBCLabel::b)  ans += "__", print_children(), ans += "__";
                else if (ptr->type == BBCLabel::url)  ans += "[", print_children(), ans += "](", ans += ptr->value, ans += ")";
                else if (ptr->type == BBCLabel::img)  ans += "![", print_children(), ans += "](", ans += ptr->value, ans += ")";
                else if (ptr->type == BBCLabel::quote) {
                    if (not ans.empty() and ans.back() != '\n')  ans += '\n';
                    ans += "> ";
                    if (not ptr->value.empty() and ptr->value.front() == '\n')  ptr->value.erase(ptr->value.begin());
                    for (auto ch: ptr->value) {
                        ans += ch;
                        if (ch == '\n')  ans += "> ";
                    }
                    ans += "\n";
                } else  print_children();
            }
        };

        dfs(dfs, &st.back());
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1783182971206534::solve();
    return 0;
}
