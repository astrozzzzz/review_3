/* ����������� ������� CalcTree10 */

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
 * @brief ��� ���� � ������ ���������.
 */
enum class NodeType {
	OPERATOR,
	NUMBER,
	VARIABLE
};

/**
 * @brief ���������, �������������� ���� � ������ ���������.
 */
struct Node {
	NodeType type;     
	char op = '\0';    
	int number = 0; 
	Node* left = nullptr;  
	Node* right = nullptr; 

	/**
	 * @brief ����������� ��� ����-�����.
	 * @param num �������� �����.
	 */
	explicit Node(int num) : type(NodeType::NUMBER), number(num) {}

	/**
	 * @brief ����������� ��� ����-���������� ('x').
	 */
	explicit Node() : type(NodeType::VARIABLE) {} // ����������� ��� 'x'

	/**
	 * @brief ����������� ��� ����-���������.
	 * @param oper ������ ���������.
	 * @param l ��������� �� ����� ������� (���������).
	 * @param r ��������� �� ������ ������� (���������).
	 */
	Node(char oper, Node* l, Node* r) : type(NodeType::OPERATOR), op(oper), left(l), right(r) {}
};

/**
 * @brief ��������� ������������ �������� � ������ ���������.
 * @param expression ������ � �������������� ����������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true, ���� ��� ������� ���������, ����� false.
 *
 * ��������� ������� ������ ����������� ��������: '+', '-', '*', '/', '%', '^', '(', ')', 'x', �����, �������.
 */
bool ValidateExpressionChars(const std::string& expression, std::ostream& error_stream);

/**
 * @brief ��������� ������������������ ������ � ���������.
 * @param expression ������ � �������������� ����������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true, ���� ������ ��������������, ����� false.
 */
bool ValidateParentheses(const std::string& expression, std::ostream& error_stream);

/**
 * @brief ���������, ��������� �� ����� � ���������� ��������� [1, 30].
 * @param number ����������� �����.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true, ���� ����� � ���������, ����� false.
 */
bool ValidateNumberRange(int number, std::ostream& error_stream);

/**
 * @brief ����������� ������� ��������� ������� ������ ��� �������.
 * @param expression ������ ���������.
 * @param error_stream ����� ������.
 * @return true, ���� ������� �������� ��������, ����� false.
 */
bool ValidateInputForParsing(const std::string& expression, std::ostream& error_stream);


// --- ������� ������ � ������� ---

/**
 * @brief ������ ��������� ��������� � ������ �� ���� ������ ���������.
 * @param expression ������ � ��������� ����������.
 * @param error_stream ����� ��� ������ ��������� �� ������� ��������.
 * @return ��������� �� ������ ������������ ������ ��� nullptr � ������ ������.
 *
 * ���������� �������� �� ������ ���� ������ (��������� � ����������) � ������
 * ����������� � ��������������� ��������. ������������ ����� [1, 30], ���������� 'x',
 * �������� +, -, *, /, %, ^ � ������.
 */
Node* ParseExpression(const std::string& expression, std::ostream& error_stream);

/**
 * @brief ���������� ��������� �������� ���������, ��������������� �������.
 * @param node ��������� �� ������� ���� ������.
 * @param x_value �������� ���������� 'x'.
 * @param error_stream ����� ��� ������ ��������� �� ������� (��������, ������� �� ����).
 * @param success ������ �� ����, ��������������� � false ��� ������ ����������.
 * @return ��������� ���������� ��������� ��� 0 � ������ ������ (success ����� false).
 *
 * ��������� �������� �������� ����� ������. ������������ �������/������� �� ����
 * � ���������� � ������� (���������� ������ ���� ���������������).
 */
int EvaluateTree(Node* node, int x_value, std::ostream& error_stream, bool& success);

/**
 * @brief ���������� ����������� ������, ������� ���������� ���� x*A �� A*x.
 * @param node ��������� �� ������� ���� ������. ������������ ������ "�� �����".
 *
 * �������� �� ������ � ��������� ��������� ������ ���, ��� ��� ���������.
 */
void TransformTree(Node* node);

/**
 * @brief ���������� �������� ������ ��������� � ������������� ������� "�� ����".
 * @param node ��������� �� ������� ���� ������.
 * @param output_stream ����� ��� ������ ������.
 * @param indent ������� ������� ������� (���������� ��������).
 *
 * ���������� �������� ����� (������ �������, ����, ����� �������) ��� ������.
 * ������ ������� ��������� 4 ������� �������. �������� ������������� �� ������� ���� � 4 ��������.
 */
void PrintTree(Node* node, std::ostream& output_stream, int indent = 0);

/**
 * @brief ���������� ������� ��� ���� ������, ���������� ������.
 * @param node ��������� �� ������ ������ (��� ���������), ������� ����� �������.
 *
 * ���������� ����������� ����� ��� ����������� �������� �����.
 */
void DeleteTree(Node* node);

// --- ������� �����/������ ---

/**
 * @brief ������ ��������� �� �����.
 * @param filename ��� ����� ��� ������.
 * @param expression ������ �� ������, ���� ����� �������� ���������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true � ������ ��������� ������, ����� false.
 */
bool ReadExpressionFromFile(const std::string& filename, std::string& expression, std::ostream& error_stream);

/**
 * @brief ����������� � ������������ �������� ���������� x.
 * @param input_stream ����� �����.
 * @param output_stream ����� ������ ��� �������.
 * @param error_stream ����� ������.
 * @param x_value ������ ��� ���������� ���������� ��������.
 * @return true � ������ ��������� ����� ������ �����, ����� false.
 */
bool ReadXValue(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream, int& x_value);

#endif 