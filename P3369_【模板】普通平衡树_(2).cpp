/**
 * @link https://www.luogu.com.cn/problem/P3369
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_5857544002385635 {
    class BST {
        struct Node {
            i32 val, cnt = 0;
            i32 size = 0;
            Node *left = nullptr, *right = nullptr;

            auto leftSize() const -> i32 {
                if (left != nullptr) return left->size;
                else return 0;
            }
            auto rightSize() const -> i32 {
                if (right != nullptr) return right->size;
                else return 0;
            }
        };

        
        std::deque<Node> nodes;
        std::vector<Node *> collected;
        Node *root = nullptr;
        
        auto allocate() -> Node * {
            if (collected.empty()) {
                nodes.emplace_back();
                return &nodes.back();
            } else {
                auto res = collected.back();
                collected.pop_back();
                return res;
            }
        }
        auto free(Node *p) -> void {
            collected.push_back(p);
            *p = Node{};
        }
        
        auto rebuild(Node *&p) -> void {
            using f64 = double;
            f64 constexpr limit = 0.75;
            i32 constexpr sizeLimit = 50;

            assert(p != nullptr);
            if (not (p->size >= sizeLimit and (
                p->leftSize() >= limit * p->size or
                p->rightSize() >= limit * p->size))) {
                return;
            }

            // 执行重构
            std::vector<std::pair<i32, i32>> tmp;
            tmp.reserve(p->size);
            auto collect = [&](auto &&collect, Node *p) -> void {
                if (p == nullptr) return;
                collect(collect, p->left);
                if (p->cnt != 0) tmp.push_back({p->val, p->cnt});
                collect(collect, p->right);
                free(p);
            };
            collect(collect, p);
            auto put = [&](auto &&put, uz first, uz last) -> Node * {
                if (last == first) return nullptr;
                auto mid = first + ((last - first) >> 1);
                auto p = allocate();
                std::tie(p->val, p->cnt) = tmp[mid];
                p->size = p->cnt;
                p->left = put(put, first, mid);
                p->right = put(put, mid + 1, last);
                if (p->left != nullptr) p->size += p->left->size;
                if (p->right != nullptr) p->size += p->right->size;
                return p;
            };
            p = put(put, 0, tmp.size());
        }

        auto insertRecursion(i32 x, Node *&p) -> void {
            if (p == nullptr) {
                p = allocate();
                p->val = x;
                p->cnt = 1, p->size = 1;
                return;
            }
            if (x < p->val) {
                insertRecursion(x, p->left);
            } else if (x > p->val) {
                insertRecursion(x, p->right);
            } else {
                p->cnt++;
            }
            p->size++;
            rebuild(p);
        }
        auto eraseRecursion(i32 x, Node *&p) -> void {
            if (p == nullptr) {
                assert(false);
                return;
            }
            if (x < p->val) {
                eraseRecursion(x, p->left);
                p->size--;
            } else if (x > p->val) {
                eraseRecursion(x, p->right);
                p->size--;
            } else {
                p->cnt--, p->size--;
            }
            rebuild(p);
        }
        auto getRankRecursion(i32 x, Node *p) -> i32 {
            if (p == nullptr) {
                return 0;
            }
            if (x < p->val) {
                return getRankRecursion(x, p->left);
            } else if (x == p->val) {
                return p->leftSize();
            } else {
                return p->leftSize() + p->cnt + getRankRecursion(x, p->right);
            }
        }
        auto getNthElementRecursion(i32 x, Node *p) -> i32 {
            if (p == nullptr) {
                assert(false);
                return 0;
            }
            if (x < p->leftSize()) {
                return getNthElementRecursion(x, p->left);
            } else if (x < p->leftSize() + p->cnt) {
                return p->val;
            } else {
                return getNthElementRecursion(x - (p->leftSize() + p->cnt), p->right);
            }
        }
        auto findRecursion(i32 x, Node *p) -> Node * {
            if (p == nullptr) return nullptr;
            if (x < p->val) {
                return findRecursion(x, p->left);
            } else if (x > p->val) {
                return findRecursion(x, p->right);
            } else {
                return p;
            }
        }
    public:
        BST() = default;

        auto insert(i32 x) -> void {
            insertRecursion(x, root);
        }
        auto erase(i32 x) -> void {
            eraseRecursion(x, root);
        }
        auto getRank(i32 x) -> i32 {
            return getRankRecursion(x, root);
        }
        auto getNthElement(i32 x) -> i32 {
            return getNthElementRecursion(x, root);
        }
        auto find(i32 x) -> Node * {
            return findRecursion(x, root);
        }
    };
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        BST t;
        for (auto _ = n; _ --> 0; ) {
            i32 op, x;
            std::cin >> op >> x;
            debug std::cerr << op << " " << x << std::endl;
            
            if (op == 1) {
                t.insert(x);
            } else if (op == 2) {
                t.erase(x);
            } else if (op == 3) {
                auto ans = t.getRank(x) + 1;
                std::cout << ans << endl;
            } else if (op == 4) {
                auto ans = t.getNthElement(x - 1);
                std::cout << ans << endl;
            } else if (op == 5) {
                auto ans = t.getNthElement(t.getRank(x) - 1);
                std::cout << ans << endl;
            } else if (op == 6) {
                auto cnt = [&]() -> i32 {
                    if (auto p = t.find(x); p != nullptr) return p->cnt;
                    else return 0;
                }();
                auto ans = t.getNthElement(t.getRank(x) + cnt);
                std::cout << ans << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5857544002385635::solve();
    return 0;
}
