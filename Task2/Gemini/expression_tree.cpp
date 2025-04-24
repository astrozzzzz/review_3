/* Продолжение задания CalcTree10: Реализация функций для работы с деревом выражения */

#include "expression_tree.h" // Сначала связанный заголовок

#include <iostream>
#include <string>
#include <vector>
#include <stack>    // Для алгоритма парсинга
#include <stdexcept> // Для обработки ошибок парсинга чисел
#include <cmath>    // Для pow
#include <cctype>   // Для isdigit, isspace
#include <limits>   // Для numeric_limits
#include <iomanip>  // Для setw, right при печати дерева
#include <fstream>  // Для работы с файлами

// Используем пространство имен std в .cpp файлах
using namespace std;

// --- Вспомогательные функции ---

/**
 * @brief Определяет приоритет оператора.
 * @param op Символ оператора.
 * @return Целочисленный приоритет (выше значение - выше приоритет).
 *         Возвращает -1 для невалидных операторов или скобок.
 */
int GetPrecedence(char op) {
	switch (op) { // Фигурные скобки на той же строке
	case '+':
	case '-':
		return 1; // Отступы в 2 таба
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3; // Возведение в степень имеет наивысший приоритет
	default:
		return -1; // Для скобок или не операторов
	}
}

/**
 * @brief Применяет операцию к верхним элементам стеков операндов и операторов.
 * @param operand_stack Стек указателей на узлы-операнды.
 * @param operator_stack Стек символов операторов.
 * @param error_stream Поток для вывода ошибок (например, нехватка операндов).
 * @return true в случае успеха, false при ошибке (например, не хватает операндов).
 *
 * Извлекает оператор и два операнда, создает новый узел-оператор
 * и помещает его обратно в стек операндов.
 * Зачем? Чтобы свернуть часть выражения в поддерево согласно приоритетам.
 */
bool ApplyOperation(stack<Node*>& operand_stack, stack<char>& operator_stack, ostream& error_stream) {
	if (operator_stack.empty()) {
		error_stream << "Ошибка парсинга: Неожиданный конец выражения или несбалансированные операторы." << endl;
		return false;
	}
	char op = operator_stack.top();
	operator_stack.pop();

	if (operand_stack.size() < 2) {
		error_stream << "Ошибка парсинга: Недостаточно операндов для операции '" << op << "'." << endl;
		// Очистка оператора, который уже извлечен
		return false;
	}
	Node* right = operand_stack.top();
	operand_stack.pop();
	Node* left = operand_stack.top();
	operand_stack.pop();

	operand_stack.push(new Node(op, left, right)); // Создаем новый узел
	return true;
}


/**
 * @brief Очищает буфер потока ввода до следующего символа новой строки.
 * @param input_stream Поток ввода, который нужно очистить.
 */
void ClearInputStream(istream& input_stream) {
	input_stream.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Реализация Функций Валидации ---

/**
 * @brief Реализация функции проверки символов. См. expression_tree.h
 */
bool ValidateExpressionChars(const string& expression, ostream& error_stream) {
	for (/*char*/ auto c : expression) { // Используем auto для итерации по строке (char)
		if (!isspace(static_cast<unsigned char>(c)) &&
			!isdigit(static_cast<unsigned char>(c)) &&
			c != 'x' &&
			string("+-*/%^()").find(c) == string::npos) { // Проверяем наличие в строке допустимых операторов
			error_stream << "Ошибка: Недопустимый символ '" << c << "' в выражении." << endl;
			return false;
		}
	}
	return true;
}

/**
 * @brief Реализация функции проверки скобок. См. expression_tree.h
 */
bool ValidateParentheses(const string& expression, ostream& error_stream) {
	int balance = 0;
	for (/*char*/ auto c : expression) { // auto (char)
		if (c == '(') {
			balance++;
		}
		else if (c == ')') {
			balance--;
		}
		if (balance < 0) { // Закрывающая скобка без открывающей
			error_stream << "Ошибка: Несбалансированные скобки (лишняя закрывающая)." << endl;
			return false;
		}
	}
	if (balance != 0) { // Не все открывающие скобки закрыты
		error_stream << "Ошибка: Несбалансированные скобки (не хватает закрывающих)." << endl;
		return false;
	}
	return true;
}

/**
 * @brief Реализация функции проверки диапазона числа. См. expression_tree.h
 */
bool ValidateNumberRange(int number, ostream& error_stream) {
	if (number < 1 || number > 30) {
		error_stream << "Ошибка: Число " << number << " вне допустимого диапазона [1, 30]." << endl;
		return false;
	}
	return true;
}

/**
 * @brief Реализация комплексной проверки перед парсингом. См. expression_tree.h
 */
bool ValidateInputForParsing(const string& expression, ostream& error_stream) {
	if (!ValidateExpressionChars(expression, error_stream)) {
		return false;
	}
	if (!ValidateParentheses(expression, error_stream)) {
		return false;
	}
	// Можно добавить другие проверки, если необходимо
	return true;
}


// --- Реализация Функций Работы с Деревом ---

/**
 * @brief Реализация парсера выражений. См. expression_tree.h
 */
Node* ParseExpression(const string& expression, ostream& error_stream) {
	if (!ValidateInputForParsing(expression, error_stream)) {
		return nullptr; // Не начинаем парсинг, если базовая валидация не пройдена
	}

	stack<Node*> operand_stack;
	stack<char> operator_stack;

	// Функция для безопасного удаления узлов в случае ошибки парсинга
	auto cleanup_stacks = [&]() { // Используем auto для лямбды (std::function<void()>)
		while (!operand_stack.empty()) {
			DeleteTree(operand_stack.top()); // Используем уже существующую функцию удаления
			operand_stack.pop();
		}
		// Стек операторов содержит только char, очистка не нужна
	};

	size_t i = 0;
	while (i < expression.length()) {
		if (isspace(static_cast<unsigned char>(expression[i]))) {
			i++; // Пропускаем пробелы
			continue;
		}

		if (isdigit(static_cast<unsigned char>(expression[i]))) {
			// Читаем целое число
			size_t start_pos = i;
			while (i < expression.length() && isdigit(static_cast<unsigned char>(expression[i]))) {
				i++;
			}
			string num_str = expression.substr(start_pos, i - start_pos);
			try {
				int number = stoi(num_str); // Пытаемся преобразовать в int
				if (!ValidateNumberRange(number, error_stream)) {
					cleanup_stacks();
					return nullptr; // Ошибка диапазона числа
				}
				operand_stack.push(new Node(number)); // Создаем узел-число
				// --- ИЗМЕНЕНИЕ ЗДЕСЬ (убраны имена переменных в catch) ---
			}
			catch (const out_of_range&) { // Убрано имя 'oor'
				error_stream << "Ошибка парсинга: Число '" << num_str << "' слишком велико." << endl;
				cleanup_stacks();
				return nullptr;
			}
			catch (const invalid_argument&) { // Убрано имя 'ia'
				// --- КОНЕЦ ИЗМЕНЕНИЯ ---
					// Эта ошибка не должна возникать из-за isdigit, но для полноты
				error_stream << "Критическая ошибка парсинга: Не удалось преобразовать '" << num_str << "' в число." << endl;
				cleanup_stacks();
				return nullptr;
			}
			continue; // Переходим к следующему символу после числа
		}

		if (expression[i] == 'x') {
			operand_stack.push(new Node()); // Создаем узел-переменную
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
					return nullptr; // Ошибка при применении операции
				}
			}
			if (operator_stack.empty()) { // Не нашли открывающую скобку
				error_stream << "Ошибка парсинга: Лишняя закрывающая скобка." << endl;
				cleanup_stacks();
				return nullptr;
			}
			operator_stack.pop(); // Удаляем '('
			i++;
			continue;
		}

		// Если это оператор
		char current_op = expression[i];
		int current_precedence = GetPrecedence(current_op);
		if (current_precedence == -1) { // Неизвестный символ (хотя ValidateExpressionChars должна была отсеять)
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
		operator_stack.push(current_op); // Помещаем текущий оператор в стек
		i++;
	} // конец while по строке выражения

	// Обрабатываем оставшиеся операторы в стеке
	while (!operator_stack.empty()) {
		if (operator_stack.top() == '(') { // Осталась незакрытая скобка
			error_stream << "Ошибка парсинга: Не хватает закрывающей скобки." << endl;
			cleanup_stacks();
			return nullptr;
		}
		if (!ApplyOperation(operand_stack, operator_stack, error_stream)) {
			cleanup_stacks();
			return nullptr;
		}
	}

	// В конце в стеке операндов должен остаться ровно один элемент - корень дерева
	if (operand_stack.size() == 1) {
		return operand_stack.top();
	}
	else {
		error_stream << "Ошибка парсинга: Некорректное выражение (проблема с количеством операндов/операторов)." << endl;
		cleanup_stacks(); // Удаляем все, что успели насоздавать
		return nullptr;
	}
}

/**
 * @brief Реализация функции вычисления дерева. См. expression_tree.h
 */
int EvaluateTree(Node* node, int x_value, ostream& error_stream, bool& success) {
	if (!node) { // Если узел пустой (например, из-за ошибки парсинга)
		error_stream << "Ошибка вычисления: Попытка вычислить пустое поддерево." << endl;
		success = false;
		return 0; // Возвращаем 0, но success = false
	}

	switch (node->type) { // Фигурные скобки на той же строке
	case NodeType::NUMBER:
		return node->number;
	case NodeType::VARIABLE:
		return x_value;
	case NodeType::OPERATOR: { // Используем блок для объявления локальных переменных
		int left_val = EvaluateTree(node->left, x_value, error_stream, success);
		if (!success) return 0;

		int right_val = EvaluateTree(node->right, x_value, error_stream, success);
		if (!success) return 0;

		switch (node->op) {
		case '+': return left_val + right_val;
		case '-': return left_val - right_val;
		case '*': return left_val * right_val;
		case '/': // Деление нацело
		case '%': // Остаток от деления
			if (right_val == 0) {
				error_stream << "Ошибка вычисления: Деление или остаток от деления на ноль (" << left_val << node->op << right_val << ")." << endl;
				success = false;
				return 0;
			}
			return (node->op == '/') ? (left_val / right_val) : (left_val % right_val);
			// --- ИЗМЕНЕНИЕ ЗДЕСЬ (добавлены скобки для case '^') ---
		case '^': { // Возведение в степень (Добавлен блок {})
			if (right_val < 0) {
				error_stream << "Ошибка вычисления: Отрицательный показатель степени (" << left_val << "^" << right_val << ")." << endl;
				success = false;
				return 0;
			}
			if (right_val == 0) return 1;
			// Объявление result_pow теперь внутри этого блока
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
		} // Конец блока для case '^'
		// --- КОНЕЦ ИЗМЕНЕНИЯ ---
		default:
			error_stream << "Критическая ошибка вычисления: Неизвестный оператор '" << node->op << "' в дереве." << endl;
			success = false;
			return 0;
		}
	} // Конец блока для case NodeType::OPERATOR
	default: // Неизвестный тип узла
		error_stream << "Критическая ошибка вычисления: Неизвестный тип узла в дереве." << endl;
		success = false;
		return 0;
	}
}

// ... (остальные функции без изменений: TransformTree, PrintTree, DeleteTree, ReadExpressionFromFile, ReadXValue) ...

/**
 * @brief Реализация функции преобразования дерева. См. expression_tree.h
 */
void TransformTree(Node* node) {
	if (!node) { // Базовый случай рекурсии
		return;
	}

	// Сначала рекурсивно преобразуем потомков
	TransformTree(node->left);
	TransformTree(node->right);

	// Проверяем условие для текущего узла ПОСЛЕ обработки потомков
	// Зачем? Чтобы не пропустить трансформацию, если потомок сам является 'x' или результатом трансформации.
	if (node->type == NodeType::OPERATOR && node->op == '*' && node->left && node->left->type == NodeType::VARIABLE) {
		// Условие x * A выполняется
		// Меняем местами левого и правого потомка
		Node* temp = node->left;
		node->left = node->right;
		node->right = temp;
	}
}

/**
 * @brief Реализация функции печати дерева. См. expression_tree.h
 */
void PrintTree(Node* node, ostream& output_stream, int indent /* = 0 */) {
	if (!node) { // Базовый случай рекурсии
		return;
	}

	const int indent_step = 4; // Отступ для каждого уровня

	// 1. Рекурсивно печатаем правое поддерево с большим отступом
	PrintTree(node->right, output_stream, indent + indent_step);

	// 2. Печатаем текущий узел с текущим отступом
	output_stream << string(indent, ' '); // Создаем строку из пробелов для отступа
	output_stream << right << setw(indent_step); // Выравнивание по правому краю в 4 позициях

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

	// 3. Рекурсивно печатаем левое поддерево с большим отступом
	PrintTree(node->left, output_stream, indent + indent_step);
}

/**
 * @brief Реализация функции удаления дерева. См. expression_tree.h
 */
void DeleteTree(Node* node) {
	if (!node) { // Базовый случай рекурсии
		return;
	}
	// Удаляем сначала потомков (постфиксный обход)
	DeleteTree(node->left);
	DeleteTree(node->right);
	// Затем удаляем сам узел
	delete node;
}

// --- Реализация Функций Ввода/Вывода ---

/**
 * @brief Реализация функции чтения из файла. См. expression_tree.h
 */
bool ReadExpressionFromFile(const string& filename, string& expression, ostream& error_stream) {
	ifstream input_file(filename); // Открываем файл для чтения
	if (!input_file.is_open()) { // Проверяем, открылся ли файл
		error_stream << "Ошибка: Не удалось открыть входной файл '" << filename << "'." << endl;
		return false;
	}

	string line;
	expression = ""; // Очищаем строку перед чтением
	bool first_line = true;
	while (getline(input_file, line)) {
		if (!first_line) {
			expression += " "; // Добавляем пробел между строками, если их несколько
		}
		expression += line;
		first_line = false;
	}

	input_file.close(); // Явно закрываем файл

	if (expression.empty()) {
		error_stream << "Ошибка: Входной файл '" << filename << "' пуст." << endl;
		return false;
	}

	return true;
}

/**
 * @brief Реализация функции чтения значения x. См. expression_tree.h
 */
bool ReadXValue(istream& input_stream, ostream& output_stream, ostream& error_stream, int& x_value) {
	while (true) { // Фигурные скобки на той же строке
		output_stream << "Введите целое значение для переменной x: ";
		if (input_stream >> x_value) {
			string remaining;
			getline(input_stream, remaining); // Читаем остаток строки
			bool only_whitespace = true;
			for (/*char*/ auto c : remaining) { // auto (char)
				if (!isspace(static_cast<unsigned char>(c))) {
					only_whitespace = false;
					break;
				}
			}

			if (only_whitespace) {
				return true; // Ввод корректен
			}
			else {
				error_stream << "Ошибка: После числа введены лишние символы. Попробуйте снова." << endl;
			}

		}
		else {
			error_stream << "Ошибка: Введено не целое число. Попробуйте снова." << endl;
			input_stream.clear(); // Сброс флагов ошибок
			ClearInputStream(input_stream); // Очистка буфера
		}
	}
}