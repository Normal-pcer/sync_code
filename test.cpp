#include <cctype>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


using namespace std;

struct Token {
    enum Type {
        KEYWORD, IDENT, CONST, OP, SYMBOL, END
    };
    Type type;
    string value;
    Token(Type t, const string& v) : type(t), value(v) {}
};

vector<Token> tokens;
size_t pos = 0;

queue<int> input_data;
ostringstream output_buffer;

struct Variable {
    bool is_array;
    vector<int> dimensions;
    vector<int> data;

    Variable() : is_array(false), dimensions(), data(1, 0) {}
    Variable(const vector<int>& dims) : is_array(true), dimensions(dims) {
        int size = 1;
        for (int d : dims) size *= d;
        data.resize(size, 0);
    }

    int get(const vector<int>& indices) {
        int index = 0, stride = 1;
        for (int i = indices.size() - 1; i >= 0; --i) {
            index += indices[i] * stride;
            stride *= dimensions[i];
        }
        return data[index];
    }

    void set(const vector<int>& indices, int value) {
        int index = 0, stride = 1;
        for (int i = indices.size() - 1; i >= 0; --i) {
            index += indices[i] * stride;
            stride *= dimensions[i];
        }
        data[index] = value;
    }
};

struct Scope {
    Scope* parent;
    unordered_map<string, Variable> variables;

    Scope(Scope* p = nullptr) : parent(p) {}

    Variable* find(const string& name) {
        Scope* current = this;
        while (current) {
            auto it = current->variables.find(name);
            if (it != current->variables.end()) return &it->second;
            current = current->parent;
        }
        return nullptr;
    }
};

struct Function {
    vector<string> params;
    struct Block;
    Block* body;
};

struct Expr {
    virtual int evaluate(Scope* scope) = 0;
    virtual ~Expr() {}
};

struct ConstExpr : Expr {
    int value;
    ConstExpr(int v) : value(v) {}
    int evaluate(Scope*) override { return value; }
};

struct VarExpr : Expr {
    string name;
    vector<unique_ptr<Expr>> indices;

    VarExpr(const string& n) : name(n) {}
    int evaluate(Scope* scope) override {
        Variable* var = scope->find(name);
        if (!var) return 0;
        vector<int> idx;
        for (auto& e : indices) idx.push_back(e->evaluate(scope));
        return var->get(idx);
    }
};

struct AssignExpr : Expr {
    unique_ptr<VarExpr> var;
    unique_ptr<Expr> value_expr;

    AssignExpr(unique_ptr<VarExpr> v, unique_ptr<Expr> e) : var(move(v)), value_expr(move(e)) {}
    int evaluate(Scope* scope) override {
        int value = value_expr->evaluate(scope);
        Variable* target = scope->find(var->name);
        vector<int> indices;
        for (auto& e : var->indices) indices.push_back(e->evaluate(scope));
        target->set(indices, value);
        return value;
    }
};

struct Program {
    vector<Function> functions;
    Scope global_scope;

    void run() {
        input_data.push(0);
        input_data.pop();
        for (auto& f : functions) {
            if (f.params.empty() && f.body) {
                executeBlock(f.body, &global_scope);
                break;
            }
        }
    }

    void executeBlock(struct Block* block, Scope* scope) {
        for (auto& stmt : block->stmts) {
            if (dynamic_cast<ReturnStmt*>(stmt.get())) {
                stmt->execute(scope);
                return;
            }
            stmt->execute(scope);
        }
    }
};

int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        int num;
        cin >> num;
        input_data.push(num);
    }
    cin.ignore();
    string line;
    while (getline(cin, line)) {
        // Tokenization logic here
    }
    // Parsing logic here
    Program program;
    program.run();
    cout << output_buffer.str();
    return 0;
}