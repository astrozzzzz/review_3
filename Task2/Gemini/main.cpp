/* Задание CalcTree10: Точка входа для работы с деревом выражения */

#include "expression_tree.h" // Наш заголовок
#include <iostream>
#include <string>
#include <fstream> // Для выходного файла

// Используем пространство имен std в .cpp файлах
using namespace std;

// --- Константы для имен файлов ---
// Зачем? Чтобы легко менять имена файлов в одном месте.
const string kInputFilename = "FN1.txt"; // Имя входного файла
const string kOutputFilename = "FN2.txt"; // Имя выходного файла

/**
 * @brief Основная функция программы.
 * @param argc Количество аргументов командной строки (не используется).
 * @param argv Массив аргументов командной строки (не используется).
 * @return 0 в случае успешного выполнения, 1 в случае ошибки.
 *
 * Выполняет полный цикл: чтение выражения, ввод x, парсинг, вычисление,
 * запись результата, преобразование дерева, печать дерева в файл.
 */
int main(int argc, char* argv[]) { // Фигурные скобки на той же строке
	setlocale(LC_ALL, "Russian");
	string expression;
	int x_value;
	Node* expression_tree = nullptr; // Инициализируем указатель на дерево

	cout << "--- Калькулятор Дерева Выражений (CalcTree10) ---" << endl;

	// 1. Чтение выражения из файла FN1
	cout << "Чтение выражения из файла '" << kInputFilename << "'..." << endl;
	if (!ReadExpressionFromFile(kInputFilename, expression, cerr)) {
		// Сообщение об ошибке уже выведено
		return 1; // Завершаем с ошибкой
	}
	cout << "Выражение прочитано: " << expression << endl;

	// 2. Запрос значения x у пользователя
	if (!ReadXValue(cin, cout, cerr, x_value)) {
		// Сообщение об ошибке уже выведено
		return 1;
	}
	cout << "Принято значение x = " << x_value << endl;

	// 3. Парсинг выражения и построение дерева
	cout << "Построение дерева выражения..." << endl;
	expression_tree = ParseExpression(expression, cerr);
	if (!expression_tree) {
		cerr << "Не удалось построить дерево выражения." << endl;
		// Очистка не нужна, т.к. ParseExpression сам чистит при ошибке
		return 1;
	}
	cout << "Дерево построено успешно." << endl;

	// 4. Вычисление значения выражения
	cout << "Вычисление значения выражения при x = " << x_value << "..." << endl;
	bool evaluation_success = true; // Флаг для отслеживания ошибок вычисления
	int result = EvaluateTree(expression_tree, x_value, cerr, evaluation_success);

	// 5. Открытие выходного файла FN2
	ofstream output_file(kOutputFilename);
	if (!output_file.is_open()) {
		cerr << "Ошибка: Не удалось открыть выходной файл '" << kOutputFilename << "' для записи." << endl;
		DeleteTree(expression_tree); // Освобождаем память перед выходом
		return 1;
	}
	cout << "Запись результатов в файл '" << kOutputFilename << "'..." << endl;


	// 6. Запись результата вычисления в FN2 (если вычисление успешно)
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
		// Не выходим с ошибкой, продолжим с печатью дерева
	}

	// 7. Преобразование дерева
	cout << "Преобразование дерева (x*A -> A*x)..." << endl;
	TransformTree(expression_tree);
	cout << "Преобразование завершено." << endl;

	// 8. Печать преобразованного дерева в FN2
	output_file << "\n--- Преобразованное дерево выражения ---" << endl;
	PrintTree(expression_tree, output_file); // Печатаем в файл
	output_file << "--- Конец дерева ---" << endl;
	cout << "Преобразованное дерево записано в файл." << endl;

	// 9. Очистка памяти и закрытие файла
	cout << "Очистка памяти..." << endl;
	DeleteTree(expression_tree); // Обязательно освобождаем память!
	expression_tree = nullptr; // Хорошая практика обнулять указатель после delete

	output_file.close(); // Явно закрываем файл
	cout << "Работа программы успешно завершена." << endl;

	return 0; // Успешное завершение
}