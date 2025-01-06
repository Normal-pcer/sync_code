/**
 * 
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_4717168014506019 {
    enum Operator {
        None, Add, Sub, Mul, Div
    };
    struct Element {
        enum Type {
            NumberType, OperatorType
        } type;
        int data;
        uint8_t get() const {
            if (type == NumberType)  return data;
            else  return -uint8_t(data);
        }

        bool operator< (const Element &other) const {
            return get() < other.get();
        }
        bool operator== (const Element &other) const {
            return get() == other.get();
        }
        operator std::string () const {
            std::stringstream out;
            out << (type == NumberType? "Number": "Operator") << data;
            return out.str();
        }
    };

    struct Node {
        using ChildrenType = std::array<Node *, 2>;
        using ValueType = int;
        Operator op;
        ll data[2];
        Node(Operator op): op(op) {
            new (data) ChildrenType();
        }
        Node(ValueType val): op(None) {
            new (data) ValueType(val);
        }
        ChildrenType &getChildren() { return assert(op != None), *reinterpret_cast<ChildrenType *>(data); }
        ValueType &getValue() { return assert(op == None), *reinterpret_cast<ValueType *>(data); }
        ~Node() {
            if (op != None) {
                delete reinterpret_cast<ChildrenType *>(data);
            } else {
                delete reinterpret_cast<ValueType *>(data);
            }
        }

        void simplify() {
            if (op == None)  return;
            getChildren()[0]->simplify();
            if (op == Add and getChildren()[0]->op == Add) {
                auto lr = getChildren()[0]->getChildren()[1];
                auto new_r = new Node{Add};
                new_r->getChildren() = {lr, getChildren()[1]};
                auto new_l = getChildren()[0]->getChildren()[0];
                getChildren() = {new_l, new_r};
            } else if (op == Mul and getChildren()[0]->op == Mul) {
                auto lr = getChildren()[0]->getChildren()[1];
                auto new_r = new Node{Mul};
                new_r->getChildren() = {lr, getChildren()[1]};
                auto new_l = getChildren()[0]->getChildren()[0];
                getChildren() = {new_l, new_r};
            }
            getChildren()[1]->simplify();
        }
        unsigned char get() {
            if (op == None)  return getValue();
            else  return -(unsigned char)op;
        }
        inline static const constexpr int base = 131;
        void _traverse1(ull &out) {
            out = (out * base) + get();
            if (op == None)  return;
            for (auto child: getChildren())  child->_traverse1(out);
        }
        void _traverse2(ull &out) {
            if (op != None)  getChildren()[0]->_traverse2(out);
            out = (out * base) + get();
            if (op != None)  getChildren()[1]->_traverse2(out);
        }
        ull hash() {
            ull res = 0;
            _traverse1(res);
            _traverse2(res);
            return res;
        }
        double calc() {
            if (op == None)  return getValue();
            auto x = getChildren()[0]->calc(), y = getChildren()[1]->calc();
            return [&]() {
                switch (op) {
                case Add:  return x + y;
                case Sub:  return x - y;
                case Mul:  return x * y;
                case Div:  return x / y;
                default:  return assert(false), 0.0;
                }
            }();
        }
    };
    const double eps = 1e-6;

    void solve() {
        std::vector<int> nums(4);
        for (auto &x: nums)  std::cin >> x;
        std::unordered_set<ull> set;
        auto cnt = 0;
        for (auto i = 0; i < (1 << 6); i++) {  // 四进制状压
            std::vector<Operator> ops(3);
            for (auto j = 0; j < 3; j++)  ops[j] = static_cast<Operator>(((i >> (j << 1)) & 3) + 1);
            // 枚举后缀表达式
            std::vector<Element> expr(7);
            for (auto j = 0; j < 3; j++)  expr[j] = {Element::OperatorType, static_cast<int>(ops[j])};
            for (auto j = 0; j < 4; j++)  expr[j + 3] = {Element::NumberType, nums[j]};
            std::sort(expr.begin(), expr.end());
            do {
                std::vector<Node *> stack;
                for (auto item: expr) {
                    if (item.type == item.NumberType) {
                        stack.push_back(new Node{item.data});
                    } else {
                        if (stack.size() < 2)  continue;
                        auto y = stack.back();  stack.pop_back();
                        auto x = stack.back();  stack.pop_back();

                        stack.push_back(new Node{static_cast<Operator>(item.data)});
                        stack.back()->getChildren()[0] = x;
                        stack.back()->getChildren()[1] = y;
                    }
                }
                if ((int)stack.size() != 1)  continue;
                auto &root = *stack.back();
                root.simplify();
                auto hash = root.hash();
                if (set.count(hash))  continue;
                auto res = root.calc();
                if (abs(res - 24) < eps) {
                    debug {
                        for (auto item: expr)  std::cout << (std::string)item << ' ';
                        std::cout << std::endl;
                    }
                    cnt++;
                }

                set.insert(hash);
            } while (std::next_permutation(expr.begin(), expr.end()));
        }
        std::cout << cnt << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4717168014506019::solve();
    return 0;
}
