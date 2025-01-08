#include <bits/stdc++.h>

struct Node {
    int val;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(int val): val(val) {}
    ~Node() {
        delete left;
        delete right;
    }

    // 重载 new 操作符
    void *operator new(size_t);
};

std::deque<std::array<long long, sizeof(Node) / sizeof(long long)>> pool;
void *Node::operator new(size_t size) {
    pool.push_back({});
    return &pool.back()[0];
}
int main() {
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    std::cout << root->val << std::endl;
    std::cout << root->left->val << std::endl;
    std::cout << root->right->val << std::endl;
    std::cout << root->left->left->val << std::endl;

}