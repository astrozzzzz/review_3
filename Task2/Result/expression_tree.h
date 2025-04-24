/* Продолжение задания CalcTree10 */

#ifndef EXPRESSION_TREE_EXPRESSION_TREE_H_
#define EXPRESSION_TREE_EXPRESSION_TREE_H_

#include <string>
#include <iostream>
#include <vector> 
#include <fstream> 
#include <stack>    
#include <stdexcept> 
#include <cmath>   
#include <cctype>   
#include <limits>  
#include <iomanip>  

/**
 * @brief Тип узла в дереве выражения.
 */
enum class NodeType {
	OPERATOR,
	NUMBER,
	VARIABLE
};

/**
 * @brief Структура, представляющая узел в дереве выражения.
 */
struct Node {
	NodeType type;     
	char op = '\0';    
	int number = 0; 
	Node* left = nullptr;  
	Node* right = nullptr; 

	/**
	 * @brief Конструктор для узла-числа.
	 * @param num Значение числа.
	 */
	explicit Node(int num) : type(NodeType::NUMBER), number(num) {}

	/**
	 * @brief Конструктор для узла-переменной ('x').
	 */
	explicit Node() : type(NodeType::VARIABLE) {} // Конструктор для 'x'

	/**
	 * @brief Конструктор для узла-оператора.
	 * @param oper Символ оператора.
	 * @param l Указатель на левый операнд (поддерево).
	 * @param r Указатель на правый операнд (поддерево).
	 */
	Node(char oper, Node* l, Node* r) : type(NodeType::OPERATOR), op(oper), left(l), right(r) {}
};

/**
 * @brief Проверяет корректность символов в строке выражения.
 * @param expression Строка с арифметическим выражением.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true, если все символы допустимы, иначе false.
 *
 * Проверяет наличие только разрешенных символов: '+', '-', '*', '/', '%', '^', '(', ')', 'x', цифры, пробелы.
 */
bool ValidateExpressionChars(const std::string& expression, std::ostream& error_stream);

/**
 * @brief Проверяет сбалансированность скобок в выражении.
 * @param expression Строка с арифметическим выражением.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true, если скобки сбалансированы, иначе false.
 */
bool ValidateParentheses(const std::string& expression, std::ostream& error_stream);

/**
 * @brief Проверяет, находится ли число в допустимом диапазоне [1, 30].
 * @param number Проверяемое число.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true, если число в диапазоне, иначе false.
 */
bool ValidateNumberRange(int number, std::ostream& error_stream);

/**
 * @brief Комплексная функция валидации входных данных для парсера.
 * @param expression Строка выражения.
 * @param error_stream Поток ошибок.
 * @return true, если базовые проверки пройдены, иначе false.
 */
bool ValidateInputForParsing(const std::string& expression, std::ostream& error_stream);


// --- Функции Работы с Деревом ---

/**
 * @brief Парсит инфиксное выражение и строит из него дерево выражения.
 * @param expression Строка с инфиксным выражением.
 * @param error_stream Поток для вывода сообщений об ошибках парсинга.
 * @return Указатель на корень построенного дерева или nullptr в случае ошибки.
 *
 * Использует алгоритм на основе двух стеков (операндов и операторов) с учетом
 * приоритетов и ассоциативности операций. Обрабатывает числа [1, 30], переменную 'x',
 * операции +, -, *, /, %, ^ и скобки.
 */
Node* ParseExpression(const std::string& expression, std::ostream& error_stream);

/**
 * @brief Рекурсивно вычисляет значение выражения, представленного деревом.
 * @param node Указатель на текущий узел дерева.
 * @param x_value Значение переменной 'x'.
 * @param error_stream Поток для вывода сообщений об ошибках (например, деление на ноль).
 * @param success Ссылка на флаг, устанавливаемый в false при ошибке вычисления.
 * @return Результат вычисления поддерева или 0 в случае ошибки (success будет false).
 *
 * Выполняет операции согласно узлам дерева. Обрабатывает деление/остаток на ноль
 * и возведение в степень (показатель должен быть неотрицательным).
 */
int EvaluateTree(Node* node, int x_value, std::ostream& error_stream, bool& success);

/**
 * @brief Рекурсивно преобразует дерево, заменяя поддеревья вида x*A на A*x.
 * @param node Указатель на текущий узел дерева. Модифицирует дерево "на месте".
 *
 * Проходит по дереву и выполняет указанную замену там, где это применимо.
 */
void TransformTree(Node* node);

/**
 * @brief Рекурсивно печатает дерево выражения в многострочном формате "на боку".
 * @param node Указатель на текущий узел дерева.
 * @param output_stream Поток для вывода дерева.
 * @param indent Текущий уровень отступа (количество пробелов).
 *
 * Использует обратный обход (правый потомок, узел, левый потомок) для печати.
 * Каждый уровень добавляет 4 пробела отступа. Значения выравниваются по правому краю в 4 позициях.
 */
void PrintTree(Node* node, std::ostream& output_stream, int indent = 0);

/**
 * @brief Рекурсивно удаляет все узлы дерева, освобождая память.
 * @param node Указатель на корень дерева (или поддерева), которое нужно удалить.
 *
 * Использует постфиксный обход для безопасного удаления узлов.
 */
void DeleteTree(Node* node);

// --- Функции Ввода/Вывода ---

/**
 * @brief Читает выражение из файла.
 * @param filename Имя файла для чтения.
 * @param expression Ссылка на строку, куда будет записано выражение.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true в случае успешного чтения, иначе false.
 */
bool ReadExpressionFromFile(const std::string& filename, std::string& expression, std::ostream& error_stream);

/**
 * @brief Запрашивает у пользователя значение переменной x.
 * @param input_stream Поток ввода.
 * @param output_stream Поток вывода для запроса.
 * @param error_stream Поток ошибок.
 * @param x_value Ссылка для сохранения введенного значения.
 * @return true в случае успешного ввода целого числа, иначе false.
 */
bool ReadXValue(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream, int& x_value);

#endif 