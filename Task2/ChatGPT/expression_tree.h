/* Продолжение задания №10 */

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/**
 * @brief Узел дерева выражения
 */
struct ExprNode {
	string value;
	ExprNode* left;
	ExprNode* right;
	ExprNode(const string& val);
	~ExprNode();
};

/**
 * @brief Проверяет корректность входных данных
 * @param expr строка с выражением
 * @return true если данные корректны, иначе false
 */
bool ValidateInput(const string& expr);

/**
 * @brief Строит дерево выражения из инфиксной строки
 * @param expr строка с выражением
 * @return корень дерева
 */
ExprNode* BuildExpressionTree(const string& expr);

/**
 * @brief Вычисляет значение выражения при заданном x
 * @param root корень дерева
 * @param x значение переменной x
 * @return результат вычисления
 */
int EvaluateTree(ExprNode* root, int x);

/**
 * @brief Преобразует дерево: x*A -> A*x
 * @param root корень дерева
 */
void TransformTree(ExprNode* root);

/**
 * @brief Печатает дерево в файл в многострочном формате
 * @param root корень дерева
 * @param out выходной поток
 * @param level уровень дерева (для отступов)
 */
void PrintTree(ExprNode* root, ofstream& out, int level = 0);