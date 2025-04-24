/**
 * Graf1. Дано описание неориентированного графа в текстовом файле с именем FileName. в виде
 * матрицы смежности. Первая строка файла содержит количество вершин графа (n), а следующие
 * n строк содержат матрицу смежности (m), m[i][j]=0, если ребра между вершинами i и j не
 * существует. Определить степень для каждой вершины графа. Вывести степени вершин,
 * перечисляя ихв порядке возрастания номеров вершин. Если в графе имеются петли, то каждая петля в степени
 * вершины учитывается дважды.
*/

#include "graph_utils.h"

using namespace std;

const string kInputFilename = "graph_input.txt";

int main(int argc, char* argv[]) {
	string filename = kInputFilename;

	if (argc > 2) {
		cerr << "Ошибка: Слишком много аргументов командной строки." << endl;
		cerr << "Использование: " << argv[0] << " [имя_входного_файла]" << endl;
		return 1;
	}
	if (argc == 2) {
		filename = argv[1];
	}

	cout << "--- Определение степеней вершин графа ---" << endl;
	cout << "Чтение графа из файла: " << filename << endl;

	vector<vector<int>> adjacency_matrix;
	vector<int> vertex_degrees;

	if (!ReadAdjacencyMatrix(filename, adjacency_matrix, cerr)) {
		return 1;
	}
	cout << "Матрица смежности успешно прочитана (" << adjacency_matrix.size() << " вершин)." << endl;

	if (!CalculateVertexDegrees(adjacency_matrix, vertex_degrees, cerr)) {
		return 1;
	}
	cout << "Степени вершин вычислены." << endl;

	PrintVertexDegrees(vertex_degrees, cout);

	cout << "Работа программы успешно завершена." << endl;
	return 0;
}