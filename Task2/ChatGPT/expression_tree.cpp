/* Продолжение задания №10 */

#include "expression_tree.h"
#include <sstream>
#include <stack>
#include <cctype>
#include <cmath>

/**
 * @brief Конструктор узла дерева выражения
 * @param val Значение узла (операция, число или переменная)
 * @details Создает новый узел дерева с указанным значением и устанавливает указатели на левое и правое поддерево в nullptr
 */
ExprNode::ExprNode(const string& val) : value(val), left(nullptr), right(nullptr) {}

/**
 * @brief Деструктор узла дерева выражения
 * @details Рекурсивно освобождает память, занятую всеми узлами поддерева
 */
ExprNode::~ExprNode() {
	delete left;
	delete right;
}

/**
 * @brief Проверяет, является ли строка целым числом
 * @param s Проверяемая строка
 * @return true если строка представляет целое число, false в противном случае
 * @details Проверяет, что строка не пуста и содержит только цифры
 */
bool IsInteger(const string& s) {
	if (s.empty()) return false;
	for (char c : s) {
		if (!isdigit(c)) return false;
	}
	return true;
}

/**
 * @brief Проверяет корректность входных данных
 * @param expr Входное выражение для проверки
 * @return true если выражение корректно, false в противном случае
 * @details Проверяет:
 *          - Допустимость всех символов в выражении
 *          - Корректность расстановки скобок
 *          - Баланс открывающих и закрывающих скобок
 */
bool ValidateInput(const string& expr) {
	// Проверяем допустимые символы и корректность скобок
	int balance = 0;
	for (char c : expr) {
		if (!(isdigit(c) || c == 'x' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(' || c == ')' || isspace(c))) {
			return false;
		}
		if (c == '(') balance++;
		if (c == ')') balance--;
		if (balance < 0) return false;
	}
	if (balance != 0) return false;
	return true;
}

/**
 * @brief Возвращает приоритет оператора
 * @param op Символ оператора
 * @return Числовое значение приоритета (3 - высший, 0 - низший)
 * @details Приоритеты операторов:
 *          - 3: возведение в степень (^)
 *          - 2: умножение (*), деление (/), остаток от деления (%)
 *          - 1: сложение (+), вычитание (-)
 *          - 0: для всех остальных символов
 */
int Priority(char op) {
	if (op == '^') return 3;
	if (op == '*' || op == '/' || op == '%') return 2;
	if (op == '+' || op == '-') return 1;
	return 0;
}

/**
 * @brief Преобразует инфиксное выражение в постфиксное
 * @param expr Входное выражение в инфиксной форме
 * @return Вектор токенов в постфиксной записи
 * @details Реализует алгоритм сортировочной станции (алгоритм Дейкстры):
 *          - Числа и переменная x добавляются сразу в выходной массив
 *          - Операторы обрабатываются с учетом их приоритета
 *          - Скобки обрабатываются особым образом для сохранения порядка операций
 */
vector<string> InfixToPostfix(const string& expr) {
	vector<string> output;
	stack<char> ops;
	istringstream iss(expr);
	string token;
	while (iss >> token) {
		if (IsInteger(token) || token == "x") {
			output.push_back(token);
		} else if (token == "(") {
			ops.push('(');
		} else if (token == ")") {
			while (!ops.empty() && ops.top() != '(') {
				output.push_back(string(1, ops.top()));
				ops.pop();
			}
			if (!ops.empty()) ops.pop();
		} else if (token.size() == 1 && string("+-*/%^").find(token[0]) != string::npos) {
			char op = token[0];
			while (!ops.empty() && Priority(ops.top()) >= Priority(op)) {
				output.push_back(string(1, ops.top()));
				ops.pop();
			}
			ops.push(op);
		}
	}
	while (!ops.empty()) {
		output.push_back(string(1, ops.top()));
		ops.pop();
	}
	return output;
}

/**
 * @brief Строит дерево выражения из инфиксной строки
 * @param expr Входное выражение в инфиксной форме
 * @return Указатель на корень построенного дерева
 * @details Алгоритм построения:
 *          1. Добавляет пробелы между операторами и операндами
 *          2. Преобразует выражение в постфиксную форму
 *          3. Строит дерево по постфиксной записи:
 *             - Числа и переменные становятся листьями
 *             - Операторы формируют внутренние узлы с двумя потомками
 */
ExprNode* BuildExpressionTree(const string& expr) {
	// Разбиваем выражение на токены с пробелами между операторами и числами
	string spaced;
	for (size_t i = 0; i < expr.size(); ++i) {
		char c = expr[i];
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(' || c == ')') {
			spaced += ' ';
			spaced += c;
			spaced += ' ';
		} else {
			spaced += c;
		}
	}
	vector<string> postfix = InfixToPostfix(spaced);
	stack<ExprNode*> st;
	for (const string& token : postfix) {
		if (IsInteger(token) || token == "x") {
			st.push(new ExprNode(token));
		} else {
			ExprNode* right = st.top(); st.pop();
			ExprNode* left = st.top(); st.pop();
			ExprNode* node = new ExprNode(token);
			node->left = left;
			node->right = right;
			st.push(node);
		}
	}
	return st.top();
}

/**
 * @brief Вычисляет значение выражения при заданном x
 * @param root Указатель на корень дерева выражения
 * @param x Значение переменной x
 * @return Результат вычисления выражения
 * @details Рекурсивно вычисляет значение выражения:
 *          - Для листьев возвращает число или значение x
 *          - Для внутренних узлов применяет соответствующую операцию
 *          - Обрабатывает особые случаи: деление на 0, отрицательные степени
 */
int EvaluateTree(ExprNode* root, int x) {
	if (!root) return 0;
	if (root->value == "x") return x;
	if (IsInteger(root->value)) return stoi(root->value);
	int l = EvaluateTree(root->left, x);
	int r = EvaluateTree(root->right, x);
	if (root->value == "+") return l + r;
	if (root->value == "-") return l - r;
	if (root->value == "*") return l * r;
	if (root->value == "/") {
		if (r == 0) {
			cerr << "Деление на ноль" << endl;
			return 0;
		}
		return l / r;
	}
	if (root->value == "%") {
		if (r == 0) {
			cerr << "Деление на ноль" << endl;
			return 0;
		}
		return l % r;
	}
	if (root->value == "^") {
		if (r < 0) {
			cerr << "Отрицательная степень" << endl;
			return 0;
		}
		return static_cast<int>(pow(l, r));
	}
	return 0;
}

/**
 * @brief Преобразует дерево, заменяя все поддеревья вида x*A на A*x
 * @param root Указатель на корень дерева
 * @details Рекурсивно обходит дерево и выполняет замену:
 *          - Сначала обрабатывает левое и правое поддерево
 *          - При обнаружении умножения проверяет, является ли левый операнд x
 *          - Если да, меняет местами поддеревья
 */
void TransformTree(ExprNode* root) {
	if (!root) return;
	TransformTree(root->left);
	TransformTree(root->right);
	if (root->value == "*") {
		if (root->left && root->left->value == "x" && root->right) {
			// Меняем местами x и поддерево
			ExprNode* tmp = root->left;
			root->left = root->right;
			root->right = tmp;
		}
	}
}

/**
 * @brief Печатает дерево в файл в многострочном формате
 * @param root Указатель на корень дерева
 * @param out Поток вывода
 * @param level Текущий уровень вложенности (для отступов)
 * @details Выводит дерево "положенным на бок":
 *          - Каждый уровень смещается на 4 позиции вправо
 *          - Правое поддерево выводится выше родителя
 *          - Левое поддерево выводится ниже родителя
 *          - Используется выравнивание по правому краю
 */
void PrintTree(ExprNode* root, ofstream& out, int level) {
	if (!root) return;
	PrintTree(root->right, out, level + 1);
	for (int i = 0; i < level; ++i) out << "    ";
	out << root->value << endl;
	PrintTree(root->left, out, level + 1);
}