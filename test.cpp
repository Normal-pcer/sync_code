/**
 * 
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4108872145884997 {
    struct Value {
        std::string label;

        auto string() const -> std::string { return label; }
    };
    struct ExprNode {
        enum Operator {
            None, Add, Mul
        } op = None;
        std::shared_ptr<ExprNode> left = nullptr;
        std::shared_ptr<ExprNode> right = nullptr;
        Value value;

        ExprNode() {}
        ExprNode(Operator op, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right): op(op), left(left), right(right) {}
        ExprNode(Value value): value(value) {}

        auto string() const -> std::string {
            if (op == None)  return value.string();
            else if (op == Add) {
                return "(" + left->string() + " + " + right->string() + ")";
            } else {
                return "(" + left->string() + " * " + right->string() + ")";
            }
        }
    };

    struct Number {
        std::shared_ptr<ExprNode> val = nullptr;

        auto operator+= (Number const &other) -> Number & {
            if (val == nullptr)  return *this = other;
            val = std::make_shared<ExprNode>(ExprNode::Add, val, other.val);
            return *this;
        }
        auto operator+ (Number const &other) const -> Number {
            auto tmp = *this;
            return tmp += other;
        }
        auto operator*= (Number const &other) -> Number & {
            if (val == nullptr)  return *this = other;
            val = std::make_shared<ExprNode>(ExprNode::Mul, val, other.val);
            return *this;
        }
        auto operator* (Number const &other) const -> Number {
            auto tmp = *this;
            return tmp *= other;
        }
        auto string() const -> std::string {
            return val->string();
        }
    };
    struct Matrix {
        i32 height, width;
        std::vector<std::vector<Number>> data;
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<Number>(width)) {}

        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res.data[i][j] += data[i][k] * other.data[k][j];
                        // std::cout << res.data[i][j].string() << std::endl;
                    }
                }
            }
            return res;
        }

        auto operator[] (uz idx) const -> std::vector<Number> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<Number> & { return data[idx]; }

        auto static named(i32 height, i32 width, std::string const &name) -> Matrix {
            Matrix res{height, width};
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < width; j++) {
                    res[i][j] = Number{std::make_shared<ExprNode>(Value{std::format("{}[{}][{}]", name, i, j)})};
                }
            }
            return res;
        }

        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            std::vector strs(mat.height, std::vector<std::string>(mat.width));
            i32 max_len = 0;
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    chkMax<i32>(max_len, (strs[i][j] = mat[i][j].string()).size());
                }
            }
            
            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    os << std::setw(max_len + 2) << strs[i][j];
                }
            }

            return os;
        }
    };
    void solve() {
        auto A = Matrix::named(2, 2, "A");
        auto B = Matrix::named(2, 2, "B");
        auto C = Matrix::named(2, 2, "C");
        std::cout << A * B * C << endl;
        std::cout << "-----" << endl;
        std::cout << A * (B * C) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4108872145884997::solve();
    return 0;
}
