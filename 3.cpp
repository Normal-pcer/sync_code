/**
 * P8815 [CSP-J 2022] 逻辑表达式
 */
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <utility>
#include <regex>
#define DEBUG false

using std::string;

enum StatusType
{
    NONE,
    INITIALIZED,
    PARSING,
    PARSED,
    EVALUATING,
    EVALUATED
};

enum Operation
{
    AND = '&',
    OR = '|'
};

class ExprNode
{
public:
    StatusType status = StatusType::NONE;
    string unparsed_text = "";
    Operation operation;
    ExprNode *left = nullptr;
    ExprNode *right = nullptr;
    bool value;
    int skiped_and = 0;
    int skiped_or = 0;

    ExprNode(bool value)
    {
        this->value = value;
        this->status = StatusType::EVALUATED;
    }
    ExprNode(string text)
    {
        this->unparsed_text = text;
        this->status = StatusType::INITIALIZED;
    }
    ExprNode(Operation operation, ExprNode *left, ExprNode *right)
    {
        this->operation = operation;
        this->left = left;
        this->right = right;
        this->status = StatusType::PARSED;
    }

    void parse()
    {
        if (this->status >= StatusType::PARSED)
            return;

        string text = this->unparsed_text;

        if (text == "0")
        {
            this->value = false;
            this->status = StatusType::EVALUATED;
            return;
        }
        else if (text == "1")
        {
            this->value = true;
            this->status = StatusType::EVALUATED;
            return;
        }

        string temp_expr = "";
        std::stack<char> operations;

        for (int i = 0; i < text.size(); i++)
        {
            char c = text[i];

            switch (c)
            {
            case '1':
                temp_expr += '1';
                break;
            case '0':
                temp_expr += '0';
                break;
            case '(':
                operations.push('(');
                break;
            case ')':
                while (!operations.empty() && operations.top() != '(')
                {
                    temp_expr += operations.top();
                    operations.pop();
                }
                operations.pop();
                break;
            case '&':
                while (!operations.empty() && operations.top() == '&')
                {
                    temp_expr += operations.top();
                    operations.pop();
                }
                operations.push('&');
                break;
            case '|':
                while (!operations.empty() && operations.top() != '(')
                {
                    temp_expr += operations.top();
                    operations.pop();
                }
                operations.push('|');
                break;
            }
        }
        while (!operations.empty())
        {
            temp_expr += operations.top();
            operations.pop();
        }

#if DEBUG 
        std::cout << unparsed_text << "==>" << temp_expr << std::endl;
#endif

        std::stack <ExprNode*> nodes;
        ExprNode *new_right = nullptr;
        ExprNode *new_left = nullptr;
        for (int i = 0; i < temp_expr.size(); i++) {
            char c = temp_expr[i];

            switch (c) {
            case '1':
                nodes.push(new ExprNode(true));
                break;
            case '0':
                nodes.push(new ExprNode(false));
                break;
            case '&':
                new_right = nodes.top();
                nodes.pop();
                new_left = nodes.top();
                nodes.pop();
                nodes.push(new ExprNode(Operation::AND, new_left, new_right));
                break;
            case '|':
                new_right = nodes.top();
                nodes.pop();
                new_left = nodes.top();
                nodes.pop();
                nodes.push(new ExprNode(Operation::OR, new_left, new_right));
                break;
            }
        }

        this->operation = nodes.top()->operation;
        this->left = nodes.top()->left;
        this->right = nodes.top()->right;

        this->status = StatusType::PARSED;
    }

    ExprNode *getLeft()
    {
        if (this->left == nullptr)
        {
            this->left = new ExprNode(false);
        }
        return this->left;
    }

    ExprNode *getRight()
    {
        if (this->right == nullptr)
        {
            this->right = new ExprNode(false);
        }
        return this->right;
    }

    void evaluate()
    {
        if (this->status < StatusType::PARSED)
            this->parse();
        if (this->status >= StatusType::EVALUATED)
            return;

        this->status = StatusType::EVALUATING;

        bool leftValue = this->getLeft()->getValue();

        if (this->operation == Operation::AND)
        {
            this->value = leftValue && this->getRight()->getValue();
            this->skiped_and += (leftValue ? 0 : 1);
        }
        else
        {
            this->value = leftValue || this->getRight()->getValue();
            this->skiped_or += (leftValue ? 1 : 0);
        }
        this->skiped_and += this->getLeft()->skiped_and + this->getRight()->skiped_and;
        this->skiped_or += this->getLeft()->skiped_or + this->getRight()->skiped_or;

        this->status = StatusType::EVALUATED;
    }

    bool getValue()
    {
        if (this->status < StatusType::EVALUATED)
            this->evaluate();
        return this->value;
    }
};

int main(int argc, char const *argv[])
{
    string given_expr = "(0|1&0|1|1|(1|1))&(0&1&(1|0)|0|1|0)&0";
#if !DEBUG
    std::cin >> given_expr;
#endif
    ExprNode *root = new ExprNode(given_expr);
    root->evaluate();
    std::cout << (root->value ? 1 : 0) << std::endl;
    std::cout << root->skiped_and << " " << root->skiped_or << std::endl;
    return 0;
}