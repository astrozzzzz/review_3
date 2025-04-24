/**
* CalcTree10. В текстовом файле с именем FN1 дано арифметическое выражение в инфиксной
* форме. В выражении могут использоваться операции: сложение(+), вычитание(-), умножение(*),
* деление нацело(/), остаток от деления(%), возведение в степень(^), а так же целые числа из
* промежутка [1; 30] и переменная x. Для операции возведения в степень показатель степени
* неотрицательное целое число. Постройте дерево выражения. После этого вычислите значение выражения при
* заданном значении переменной x и выведите результат в текстовый файл с именем FN2.
* Преобразуйте дерево, заменив все поддеревья вида x*A на A*x, где A - некоторое поддерево, а x
* - переменная. Распечатайте дерево после преобразования в файл FN2 используя многострочный
* формат, в котором дерево положено на бок. Каждый уровень дерева выводите в 4-х позициях и
* используйте выравнивание по правому краю. При наличии нескольких подряд идущих одинаковых
* операций дерево должно строиться по правилу: операции одинакового приоритета вычисляются по
* порядку слева направо. Иными словами, выражение 2+3+4+5, например, должно трактоваться как ((2+3)+4)+5, и
* не может трактоваться как (2+3)+(4+5) или 2+(3+(4+5)). Результаты всех
* вычислений, включая промежуточные, принадлежат типу int.
*/

#include "expression_tree.h" 

using namespace std;


const string kInputFilename = "FN1.txt"; 
const string kOutputFilename = "FN2.txt"; 

/**
 * @brief Основная функция программы.
 * @param argc Количество аргументов командной строки (не используется).
 * @param argv Массив аргументов командной строки (не используется).
 * @return 0 в случае успешного выполнения, 1 в случае ошибки.
 *
 * Выполняет полный цикл: чтение выражения, ввод x, парсинг, вычисление,
 * запись результата, преобразование дерева, печать дерева в файл.
 */
int main(int argc, char* argv[]) { 
	setlocale(LC_ALL, "Russian");
	string expression = "";
	int x_value = 1;
	Node* expression_tree = nullptr; 

	cout << "--- Калькулятор Дерева Выражений (CalcTree10) ---" << endl;

	cout << "Чтение выражения из файла '" << kInputFilename << "'..." << endl;
	if (!ReadExpressionFromFile(kInputFilename, expression, cerr)) {
		return 1; 
	}
	cout << "Выражение прочитано: " << expression << endl;

	if (!ReadXValue(cin, cout, cerr, x_value)) {
		return 1;
	}
	cout << "Принято значение x = " << x_value << endl;

	cout << "Построение дерева выражения..." << endl;
	expression_tree = ParseExpression(expression, cerr);
	if (!expression_tree) {
		cerr << "Не удалось построить дерево выражения." << endl;
		return 1;
	}
	cout << "Дерево построено успешно." << endl;

	cout << "Вычисление значения выражения при x = " << x_value << "..." << endl;
	bool evaluation_success = true; 
	int result = EvaluateTree(expression_tree, x_value, cerr, evaluation_success);

	ofstream output_file(kOutputFilename);
	if (!output_file.is_open()) {
		cerr << "Ошибка: Не удалось открыть выходной файл '" << kOutputFilename << "' для записи." << endl;
		DeleteTree(expression_tree); 
		return 1;
	}
	cout << "Запись результатов в файл '" << kOutputFilename << "'..." << endl;

	if (evaluation_success) {
		output_file << "Выражение: " << expression << endl;
		output_file << "Значение x: " << x_value << endl;
		output_file << "Результат вычисления: " << result << endl;
		cout << "Результат вычисления: " << result << endl;
	}
	else {
		output_file << "Выражение: " << expression << endl;
		output_file << "Значение x: " << x_value << endl;
		output_file << "Результат вычисления: ОШИБКА (см. консоль)" << endl;
		cerr << "Ошибка при вычислении выражения. Результат не записан как число." << endl;
	}

	cout << "Преобразование дерева (x*A -> A*x)..." << endl;
	TransformTree(expression_tree);
	cout << "Преобразование завершено." << endl;

	output_file << "\n--- Преобразованное дерево выражения ---" << endl;
	PrintTree(expression_tree, output_file); 
	output_file << "--- Конец дерева ---" << endl;
	cout << "Преобразованное дерево записано в файл." << endl;

	cout << "Очистка памяти..." << endl;
	DeleteTree(expression_tree); 
	expression_tree = nullptr; 

	output_file.close();
	cout << "Работа программы успешно завершена." << endl;

	return 0;
}