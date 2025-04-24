/* Продолжение задания CalcTree10 */

#include "expression_tree.h" 

using namespace std;


int GetPrecedence(char op) {
	switch (op) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3;
	default:
		return -1;
	}
}

bool ApplyOperation(stack<Node*>& operand_stack, stack<char>& operator_stack, ostream& error_stream) {
	if (operator_stack.empty()) {
		error_stream << "Ошибка парсинга: Неожиданный конец выражения или несбалансированные операторы." << endl;
		return false;
	}
	char op = operator_stack.top();
	operator_stack.pop();

	if (operand_stack.size() < 2) {
		error_stream << "Ошибка парсинга: Недостаточно операндов для операции '" << op << "'." << endl;
		return false;
	}
	Node* right = operand_stack.top();
	operand_stack.pop();
	Node* left = operand_stack.top();
	operand_stack.pop();

	operand_stack.push(new Node(op, left, right));
	return true;
}


void ClearInputStream(istream& input_stream) {
	input_stream.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool ValidateExpressionChars(const string& expression, ostream& error_stream) {
	for (/*char*/ auto c : expression) { 
		if (!isspace(static_cast<unsigned char>(c)) &&
			!isdigit(static_cast<unsigned char>(c)) &&
			c != 'x' &&
			string("+-*/%^()").find(c) == string::npos) {
			error_stream << "Ошибка: Недопустимый символ '" << c << "' в выражении." << endl;
			return false;
		}
	}
	return true;
}

bool ValidateParentheses(const string& expression, ostream& error_stream) {
	int balance = 0;
	for (/*char*/ auto c : expression) {
		if (c == '(') {
			balance++;
		}
		else if (c == ')') {
			balance--;
		}
		if (balance < 0) {
			error_stream << "Ошибка: Несбалансированные скобки (лишняя закрывающая)." << endl;
			return false;
		}
	}
	if (balance != 0) {
		error_stream << "Ошибка: Несбалансированные скобки (не хватает закрывающих)." << endl;
		return false;
	}
	return true;
}

bool ValidateNumberRange(int number, ostream& error_stream) {
	if (number < 1 || number > 30) {
		error_stream << "Ошибка: Число " << number << " вне допустимого диапазона [1, 30]." << endl;
		return false;
	}
	return true;
}

bool ValidateInputForParsing(const string& expression, ostream& error_stream) {
	if (!ValidateExpressionChars(expression, error_stream)) {
		return false;
	}
	if (!ValidateParentheses(expression, error_stream)) {
		return false;
	}
	return true;
}

Node* ParseExpression(const string& expression, ostream& error_stream) {
	if (!ValidateInputForParsing(expression, error_stream)) {
		return nullptr;
	}

	stack<Node*> operand_stack;
	stack<char> operator_stack;

	auto cleanup_stacks = [&]() {
		while (!operand_stack.empty()) {
			DeleteTree(operand_stack.top());
			operand_stack.pop();
		}
	};

	size_t i = 0;
	while (i < expression.length()) {
		if (isspace(static_cast<unsigned char>(expression[i]))) {
			i++;
			continue;
		}

		if (isdigit(static_cast<unsigned char>(expression[i]))) {
			size_t start_pos = i;
			while (i < expression.length() && isdigit(static_cast<unsigned char>(expression[i]))) {
				i++;
			}
			string num_str = expression.substr(start_pos, i - start_pos);
			try {
				int number = stoi(num_str);
				if (!ValidateNumberRange(number, error_stream)) {
					cleanup_stacks();
					return nullptr;
				}
				operand_stack.push(new Node(number));
			}
			catch (const out_of_range&) {
				error_stream << "Ошибка парсинга: Число '" << num_str << "' слишком велико." << endl;
				cleanup_stacks();
				return nullptr;
			}
			catch (const invalid_argument&) {
				error_stream << "Критическая ошибка парсинга: Не удалось преобразовать '" << num_str << "' в число." << endl;
				cleanup_stacks();
				return nullptr;
			}
			continue; 
		}

		if (expression[i] == 'x') {
			operand_stack.push(new Node());
			i++;
			continue;
		}

		if (expression[i] == '(') {
			operator_stack.push('(');
			i++;
			continue;
		}

		if (expression[i] == ')') {
			while (!operator_stack.empty() && operator_stack.top() != '(') {
				if (!ApplyOperation(operand_stack, operator_stack, error_stream)) {
					cleanup_stacks();
					return nullptr;
				}
			}
			if (operator_stack.empty()) { 
				error_stream << "Ошибка парсинга: Лишняя закрывающая скобка." << endl;
				cleanup_stacks();
				return nullptr;
			}
			operator_stack.pop();
			i++;
			continue;
		}

		char current_op = expression[i];
		int current_precedence = GetPrecedence(current_op);
		if (current_precedence == -1) {
			error_stream << "Ошибка парсинга: Неизвестный символ или оператор '" << current_op << "'." << endl;
			cleanup_stacks();
			return nullptr;
		}

		while (!operator_stack.empty() &&
			operator_stack.top() != '(' &&
			GetPrecedence(operator_stack.top()) >= current_precedence) {
			if (!ApplyOperation(operand_stack, operator_stack, error_stream)) {
				cleanup_stacks();
				return nullptr;
			}
		}
		operator_stack.push(current_op); 
		i++;
	} 

	while (!operator_stack.empty()) {
		if (operator_stack.top() == '(') { 
			error_stream << "Ошибка парсинга: Не хватает закрывающей скобки." << endl;
			cleanup_stacks();
			return nullptr;
		}
		if (!ApplyOperation(operand_stack, operator_stack, error_stream)) {
			cleanup_stacks();
			return nullptr;
		}
	}

	if (operand_stack.size() == 1) {
		return operand_stack.top();
	}
	else {
		error_stream << "Ошибка парсинга: Некорректное выражение (проблема с количеством операндов/операторов)." << endl;
		cleanup_stacks();
		return nullptr;
	}
}

int EvaluateTree(Node* node, int x_value, ostream& error_stream, bool& success) {
	if (!node) { 
		error_stream << "Ошибка вычисления: Попытка вычислить пустое поддерево." << endl;
		success = false;
		return 0; 
	}

	switch (node->type) { 
	case NodeType::NUMBER:
		return node->number;
	case NodeType::VARIABLE:
		return x_value;
	case NodeType::OPERATOR: {
		int left_val = EvaluateTree(node->left, x_value, error_stream, success);
		if (!success) return 0;

		int right_val = EvaluateTree(node->right, x_value, error_stream, success);
		if (!success) return 0;

		switch (node->op) {
		case '+': return left_val + right_val;
		case '-': return left_val - right_val;
		case '*': return left_val * right_val;
		case '/': 
		case '%': 
			if (right_val == 0) {
				error_stream << "Ошибка вычисления: Деление или остаток от деления на ноль (" << left_val << node->op << right_val << ")." << endl;
				success = false;
				return 0;
			}
			return (node->op == '/') ? (left_val / right_val) : (left_val % right_val);
		case '^': { 
			if (right_val < 0) {
				error_stream << "Ошибка вычисления: Отрицательный показатель степени (" << left_val << "^" << right_val << ")." << endl;
				success = false;
				return 0;
			}
			if (right_val == 0) return 1;
			long long result_pow = 1;
			for (int p = 0; p < right_val; ++p) {
				result_pow *= left_val;
				if (result_pow > numeric_limits<int>::max() || result_pow < numeric_limits<int>::min()) {
					error_stream << "Ошибка вычисления: Переполнение типа int при возведении в степень (" << left_val << "^" << right_val << ")." << endl;
					success = false;
					return 0;
				}
			}
			return static_cast<int>(result_pow);
		} 
		default:
			error_stream << "Критическая ошибка вычисления: Неизвестный оператор '" << node->op << "' в дереве." << endl;
			success = false;
			return 0;
		}
	} 
	default: 
		error_stream << "Критическая ошибка вычисления: Неизвестный тип узла в дереве." << endl;
		success = false;
		return 0;
	}
}

void TransformTree(Node* node) {
	if (!node) {
		return;
	}

	TransformTree(node->left);
	TransformTree(node->right);

	if (node->type == NodeType::OPERATOR && node->op == '*' && node->left && node->left->type == NodeType::VARIABLE) {
		Node* temp = node->left;
		node->left = node->right;
		node->right = temp;
	}
}


void PrintTree(Node* node, ostream& output_stream, int indent /* = 0 */) {
	if (!node) {
		return;
	}
	const int indent_step = 4; 
	PrintTree(node->right, output_stream, indent + indent_step);
	output_stream << string(indent, ' '); 
	output_stream << right << setw(indent_step); 
	switch (node->type) {
	case NodeType::NUMBER:
		output_stream << node->number << endl;
		break;
	case NodeType::VARIABLE:
		output_stream << 'x' << endl;
		break;
	case NodeType::OPERATOR:
		output_stream << node->op << endl;
		break;
	}

	PrintTree(node->left, output_stream, indent + indent_step);
}

void DeleteTree(Node* node) {
	if (!node) { 
		return;
	}
	DeleteTree(node->left);
	DeleteTree(node->right);
	delete node;
}

bool ReadExpressionFromFile(const string& filename, string& expression, ostream& error_stream) {
	ifstream input_file(filename); 
	if (!input_file.is_open()) { 
		error_stream << "Ошибка: Не удалось открыть входной файл '" << filename << "'." << endl;
		return false;
	}

	string line = "";
	expression = ""; 
	bool first_line = true;
	while (getline(input_file, line)) {
		if (!first_line) {
			expression += " "; 
		}
		expression += line;
		first_line = false;
	}

	input_file.close(); 

	if (expression.empty()) {
		error_stream << "Ошибка: Входной файл '" << filename << "' пуст." << endl;
		return false;
	}

	return true;
}

bool ReadXValue(istream& input_stream, ostream& output_stream, ostream& error_stream, int& x_value) {
	while (true) { 
		output_stream << "Введите целое значение для переменной x: ";
		if (input_stream >> x_value) {
			string remaining = "";
			getline(input_stream, remaining); 
			bool only_whitespace = true;
			for (/*char*/ auto c : remaining) { // auto (char)
				if (!isspace(static_cast<unsigned char>(c))) {
					only_whitespace = false;
					break;
				}
			}

			if (only_whitespace) {
				return true; 
			}
			else {
				error_stream << "Ошибка: После числа введены лишние символы. Попробуйте снова." << endl;
			}

		}
		else {
			error_stream << "Ошибка: Введено не целое число. Попробуйте снова." << endl;
			input_stream.clear(); 
			ClearInputStream(input_stream); 
		}
	}
}