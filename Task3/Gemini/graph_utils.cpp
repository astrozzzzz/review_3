/* Продолжение задания Graf1: Реализация функций для работы с графом */

#include "graph_utils.h" // Сначала связанный заголовок

#include <vector>
#include <string>
#include <iostream>
#include <fstream>  // Для ifstream
#include <sstream>  // Для istringstream
#include <limits>   // Для numeric_limits

// Используем пространство имен std в .cpp файлах
using namespace std;

// --- Вспомогательная функция (если нужна) ---
// В данном случае не требуется

// --- Реализация функций из .h ---

/**
 * @brief Реализация чтения матрицы смежности. См. graph_utils.h
 */
bool ReadAdjacencyMatrix(const string& filename,
	vector<vector<int>>& adjacency_matrix, // Не const, т.к. мы ее заполняем
	ostream& error_stream) {
	ifstream input_file(filename); // Открываем файл
	if (!input_file.is_open()) { // Проверяем открытие
		error_stream << "Ошибка: Не удалось открыть входной файл '" << filename << "'." << endl;
		return false;
	}

	int n = 0; // Количество вершин
	// Читаем первую строку - количество вершин
	if (!(input_file >> n)) {
		error_stream << "Ошибка: Не удалось прочитать количество вершин (n) из файла '" << filename << "'." << endl;
		input_file.close(); // Закрываем файл перед выходом
		return false;
	}

	// Проверяем корректность n
	if (n < 0) {
		error_stream << "Ошибка: Количество вершин (n=" << n << ") не может быть отрицательным." << endl;
		input_file.close();
		return false;
	}
	if (n > 10000) { // Добавим разумный лимит, чтобы избежать выделения огромной памяти
		error_stream << "Ошибка: Слишком большое количество вершин (n=" << n << "). Максимум 10000." << endl;
		input_file.close();
		return false;
	}

	// Очищаем и изменяем размер матрицы
	adjacency_matrix.clear();
	// Если n == 0, матрица останется пустой, что корректно
	if (n > 0) {
		try {
			adjacency_matrix.resize(n, vector<int>(n));
		}
		catch (const std::bad_alloc& ba) {
			error_stream << "Ошибка: Недостаточно памяти для создания матрицы размера " << n << "x" << n << "." << endl;
			input_file.close();
			return false;
		}
	}


	// Пропускаем остаток первой строки (возможный перевод строки)
	input_file.ignore(numeric_limits<streamsize>::max(), '\n');

	// Читаем n строк матрицы
	for (int i = 0; i < n; ++i) { // отступы в 2 таба
		string line;
		if (!getline(input_file, line)) {
			error_stream << "Ошибка: Не удалось прочитать строку " << (i + 1)
				<< " матрицы (ожидалось " << n << " строк)." << endl;
			input_file.close();
			return false;
		}

		istringstream line_stream(line); // Используем строковый поток для разбора строки
		int value;
		int count = 0; // Счетчик чисел в строке
		while (line_stream >> value) {
			if (count >= n) { // Проверяем, не больше ли чисел, чем n
				error_stream << "Ошибка: В строке " << (i + 1) << " матрицы больше "
					<< n << " чисел." << endl;
				input_file.close();
				return false;
			}
			adjacency_matrix[i][count] = value;
			count++;
		}

		// Проверяем, прочитал ли поток всю строку или остановился на не-числе
		string remaining;
		if (line_stream >> remaining) { // Если что-то осталось после чисел
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы обнаружены нечисловые данные после чисел: '" << remaining << "'" << endl;
			input_file.close();
			return false;
		}
		// Проверка, если >> остановился из-за ошибки, а не конца строки
		if (!line_stream.eof() && line_stream.fail() && !line_stream.bad()) {
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы обнаружены нечисловые данные." << endl;
			input_file.close();
			return false;
		}


		if (count != n) { // Проверяем, прочитали ли ровно n чисел
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы прочитано "
				<< count << " чисел, ожидалось " << n << "." << endl;
			input_file.close();
			return false;
		}
	}

	// Проверка на лишние строки в файле (не обязательно, но полезно)
	string extra_line;
	if (getline(input_file, extra_line) && !extra_line.empty()) {
		// Проверяем, не состоит ли строка только из пробелов
		bool only_whitespace = true;
		for (/*char*/ auto c : extra_line) { // auto -> char
			if (!isspace(static_cast<unsigned char>(c))) {
				only_whitespace = false;
				break;
			}
		}
		if (!only_whitespace) {
			error_stream << "Предупреждение: Во входном файле есть лишние непустые строки после матрицы." << endl;
			// Не возвращаем false, т.к. матрицу прочитали
		}

	}


	input_file.close(); // Явно закрываем файл
	return true; // Успешное чтение
}

/**
 * @brief Реализация вычисления степеней вершин. См. graph_utils.h
 */
bool CalculateVertexDegrees(const vector<vector<int>>& adjacency_matrix,
	vector<int>& degrees, // Не const, т.к. мы ее заполняем
	ostream& error_stream) {
	size_t n = adjacency_matrix.size();

	// Проверяем, что матрица не пустая и квадратная
	if (n == 0) {
		degrees.clear(); // Для пустого графа степени не определены или их 0
		return true; // Считаем пустой граф валидным
	}

	for (size_t i = 0; i < n; ++i) {
		if (adjacency_matrix[i].size() != n) {
			error_stream << "Ошибка: Матрица смежности не квадратная (строка " << i
				<< " имеет размер " << adjacency_matrix[i].size() << ", ожидалось " << n << ")." << endl;
			return false;
		}
	}

	// Инициализируем вектор степеней нулями
	degrees.assign(n, 0); // assign очищает и задает размер/значения

	// Вычисляем степени
	// Зачем? Чтобы посчитать количество ребер, инцидентных каждой вершине,
	// с учетом особого правила для петель.
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (adjacency_matrix[i][j] != 0) { // Если есть ребро (или петля)
				if (i == j) { // Если это петля
					degrees[i] += 2; // Петля учитывается дважды
				}
				else { // Если это обычное ребро
					degrees[i] += 1; // Добавляем 1 к степени вершины i
				}
			}
		}
	}

	return true; // Успешное вычисление
}

/**
 * @brief Реализация вывода степеней вершин. См. graph_utils.h
 */
void PrintVertexDegrees(const vector<int>& degrees, ostream& output_stream) {
	output_stream << "Степени вершин графа:" << endl;
	if (degrees.empty()) {
		output_stream << "(Граф не содержит вершин)" << endl;
		return;
	}
	for (size_t i = 0; i < degrees.size(); ++i) {
		output_stream << "Вершина " << i << ": " << degrees[i] << endl;
	}
}