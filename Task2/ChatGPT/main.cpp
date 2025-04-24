/* Продолжение задания №10 */

#include "expression_tree.h"
using namespace std;

int main() {
	// Открываем входной файл
	setlocale(LC_ALL, "Russian");
	ifstream fin("FN1.txt");
	if (!fin.is_open()) {
		cerr << "Не удалось открыть файл FN1" << endl;
		return 1;
	}

	string expr;
	getline(fin, expr);
	fin.close();

	// Проверяем корректность входных данных
	if (!ValidateInput(expr)) {
		cerr << "Некорректные входные данные" << endl;
		return 1;
	}

	// Строим дерево выражения
	ExprNode* root = BuildExpressionTree(expr);

	// Запрашиваем значение x
	int x;
	cout << "Введите значение x: ";
	cin >> x;
	if (cin.fail()) {
		cerr << "Некорректный ввод x" << endl;
		delete root;
		return 1;
	}

	// Вычисляем значение выражения
	int result = EvaluateTree(root, x);

	// Открываем выходной файл
	ofstream fout("FN2.txt");
	if (!fout.is_open()) {
		cerr << "Не удалось открыть файл FN2" << endl;
		delete root;
		return 1;
	}

	fout << "Результат вычисления: " << result << endl;

	// Преобразуем дерево
	TransformTree(root);

	fout << "Дерево выражения после преобразования:" << endl;
	PrintTree(root, fout);

	fout.close();
	delete root;
	return 0;
}