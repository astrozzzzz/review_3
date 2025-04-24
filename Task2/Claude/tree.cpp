#include "tree.h"
#include <fstream>
#include <stack>
#include <cctype>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

ExpressionTree::ExpressionTree() : root(nullptr) {}

ExpressionTree::~ExpressionTree() {
    clearTree(root);
}

void ExpressionTree::clearTree(Node* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

int ExpressionTree::getPriority(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/':
        case '%': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

vector<string> ExpressionTree::tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    
    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];
        
        if (isspace(c)) continue;
        
        if (isdigit(c)) {
            token = "";
            while (i < expression.length() && isdigit(expression[i])) {
                token += expression[i++];
            }
            i--;
            tokens.push_back(token);
        }
        else if (c == 'x') {
            tokens.push_back("x");
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || 
                 c == '(' || c == ')') {
            tokens.push_back(string(1, c));
        }
    }
    
    return tokens;
}

bool ExpressionTree::buildTree(const string& expression) {
    vector<string> tokens = tokenize(expression);
    stack<Node*> nodes;
    stack<string> ops;
    
    for (const string& token : tokens) {
        if (token == "(") {
            ops.push(token);
        }
        else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                Node* node = createNode(ops.top());
                ops.pop();
                
                if (nodes.size() < 2) {
                    cerr << "Ошибка: неверное выражение" << endl;
                    return false;
                }
                
                node->right = nodes.top(); nodes.pop();
                node->left = nodes.top(); nodes.pop();
                nodes.push(node);
            }
            if (!ops.empty()) ops.pop(); // Удаляем "("
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/" || 
                 token == "%" || token == "^") {
            while (!ops.empty() && ops.top() != "(" && 
                   getPriority(ops.top()[0]) >= getPriority(token[0])) {
                Node* node = createNode(ops.top());
                ops.pop();
                
                node->right = nodes.top(); nodes.pop();
                node->left = nodes.top(); nodes.pop();
                nodes.push(node);
            }
            ops.push(token);
        }
        else {
            nodes.push(createNode(token));
        }
    }
    
    while (!ops.empty()) {
        Node* node = createNode(ops.top());
        ops.pop();
        
        node->right = nodes.top(); nodes.pop();
        node->left = nodes.top(); nodes.pop();
        nodes.push(node);
    }
    
    if (nodes.size() != 1) {
        cerr << "Ошибка: неверное выражение" << endl;
        return false;
    }
    
    root = nodes.top();
    return true;
}

Node* ExpressionTree::createNode(const string& token) {
    return new Node(token);
}

int ExpressionTree::evaluate(int x) {
    if (x < 1 || x > 30) {
        cerr << "Ошибка: значение x должно быть в диапазоне [1; 30]" << endl;
        return 0;
    }
    return evaluateHelper(root, x);
}

int ExpressionTree::evaluateHelper(Node* node, int x) {
    if (!node) return 0;
    
    if (isdigit(node->value[0])) {
        return stoi(node->value);
    }
    
    if (node->value == "x") {
        return x;
    }
    
    int left = evaluateHelper(node->left, x);
    int right = evaluateHelper(node->right, x);
    
    if (node->value == "+") return left + right;
    if (node->value == "-") return left - right;
    if (node->value == "*") return left * right;
    if (node->value == "/") {
        if (right == 0) {
            cerr << "Ошибка: деление на ноль" << endl;
            return 0;
        }
        return left / right;
    }
    if (node->value == "%") {
        if (right == 0) {
            cerr << "Ошибка: деление на ноль" << endl;
            return 0;
        }
        return left % right;
    }
    if (node->value == "^") {
        if (right < 0) {
            cerr << "Ошибка: отрицательная степень" << endl;
            return 0;
        }
        return static_cast<int>(pow(left, right));
    }
    
    return 0;
}

void ExpressionTree::transformMultiplication() {
    transformMultiplicationHelper(root);
}

void ExpressionTree::transformMultiplicationHelper(Node* node) {
    if (!node) return;
    
    transformMultiplicationHelper(node->left);
    transformMultiplicationHelper(node->right);
    
    if (node->value == "*" && node->left && node->left->value == "x") {
        // Меняем местами x и A
        Node* temp = node->left;
        node->left = node->right;
        node->right = temp;
    }
}

void ExpressionTree::printTree(const string& filename) {
    FILE* file = fopen(filename.c_str(), "a");
    if (!file) {
        cerr << "Ошибка: не удалось открыть файл для записи" << endl;
        return;
    }
    
    printTreeHelper(root, 0, file);
    fclose(file);
}

void ExpressionTree::printTreeHelper(Node* node, int level, FILE* file) {
    if (!node) return;
    
    printTreeHelper(node->right, level + 1, file);
    
    for (int i = 0; i < level; i++) {
        fprintf(file, "    ");
    }
    fprintf(file, "%s\n", node->value.c_str());
    
    printTreeHelper(node->left, level + 1, file);
}