/* Продолжение задания Graf1 */
#include "graph_utils.h"

using namespace std;


bool ReadAdjacencyMatrix(const string& filename,
	vector<vector<int>>& adjacency_matrix,
	ostream& error_stream) {
	ifstream input_file(filename);
	if (!input_file.is_open()) {
		error_stream << "Ошибка: Не удалось открыть входной файл '" << filename << "'." << endl;
		return false;
	}
	int n = 0;
	if (!(input_file >> n)) {
		error_stream << "Ошибка: Не удалось прочитать количество вершин (n) из файла '" << filename << "'." << endl;
		input_file.close();
		return false;
	}
	if (n < 0) {
		error_stream << "Ошибка: Количество вершин (n=" << n << ") не может быть отрицательным." << endl;
		input_file.close();
		return false;
	}
	if (n > 10000) {
		error_stream << "Ошибка: Слишком большое количество вершин (n=" << n << "). Максимум 10000." << endl;
		input_file.close();
		return false;
	}
	adjacency_matrix.clear();
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
	input_file.ignore(numeric_limits<streamsize>::max(), '\n');
	for (int i = 0; i < n; ++i) {
		string line;
		if (!getline(input_file, line)) {
			error_stream << "Ошибка: Не удалось прочитать строку " << (i + 1)
				<< " матрицы (ожидалось " << n << " строк)." << endl;
			input_file.close();
			return false;
		}
		istringstream line_stream(line);
		int value = 0;
		int count = 0;
		while (line_stream >> value) {
			if (count >= n) {
				error_stream << "Ошибка: В строке " << (i + 1) << " матрицы больше "
					<< n << " чисел." << endl;
				input_file.close();
				return false;
			}
			adjacency_matrix[i][count] = value;
			count++;
		}
		string remaining = "";
		if (line_stream >> remaining) {
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы обнаружены нечисловые данные после чисел: '" << remaining << "'" << endl;
			input_file.close();
			return false;
		}
		if (!line_stream.eof() && line_stream.fail() && !line_stream.bad()) {
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы обнаружены нечисловые данные." << endl;
			input_file.close();
			return false;
		}
		if (count != n) {
			error_stream << "Ошибка: В строке " << (i + 1) << " матрицы прочитано "
				<< count << " чисел, ожидалось " << n << "." << endl;
			input_file.close();
			return false;
		}
	}
	string extra_line;
	if (getline(input_file, extra_line) && !extra_line.empty()) {
		bool only_whitespace = true;
		for (auto c : extra_line) {
			if (!isspace(static_cast<unsigned char>(c))) {
				only_whitespace = false;
				break;
			}
		}
		if (!only_whitespace) {
			error_stream << "Предупреждение: Во входном файле есть лишние непустые строки после матрицы." << endl;

		}
	}
	input_file.close();
	return true;
}

bool CalculateVertexDegrees(const vector<vector<int>>& adjacency_matrix,
	vector<int>& degrees,
	ostream& error_stream) {
	size_t n = adjacency_matrix.size();

	if (n == 0) {
		degrees.clear();
		return true;
	}

	for (size_t i = 0; i < n; ++i) {
		if (adjacency_matrix[i].size() != n) {
			error_stream << "Ошибка: Матрица смежности не квадратная (строка " << i
				<< " имеет размер " << adjacency_matrix[i].size() << ", ожидалось " << n << ")." << endl;
			return false;
		}
	}
	degrees.assign(n, 0);
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (adjacency_matrix[i][j] != 0) {
				if (i == j) {
					degrees[i] += 2;
				}
				else {
					degrees[i] += 1;
				}
			}
		}
	}
	return true;
}

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