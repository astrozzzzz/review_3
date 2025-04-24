/* Продолжение задания Graf1 */

#ifndef GRAPH_UTILS_GRAPH_UTILS_H_
#define GRAPH_UTILS_GRAPH_UTILS_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

/**
 * @brief Читает матрицу смежности неориентированного графа из файла.
 * @param filename Имя входного файла.
 * @param adjacency_matrix Ссылка на 2D вектор, куда будет записана матрица.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true в случае успешного чтения и валидации базовой структуры файла, иначе false.
 *
 * Первая строка файла должна содержать количество вершин n.
 * Следующие n строк должны содержать по n целых чисел, разделенных пробелами.
 * Функция проверяет открытие файла, корректность чтения n и размерность матрицы.
 * Не проверяет значения внутри матрицы (кроме того, что это числа).
 */
bool ReadAdjacencyMatrix(const std::string& filename,
	std::vector<std::vector<int>>& adjacency_matrix,
	std::ostream& error_stream);

/**
 * @brief Вычисляет степени всех вершин графа по его матрице смежности.
 * @param adjacency_matrix Константная ссылка на матрицу смежности графа.
 * @param degrees Ссылка на вектор, куда будут записаны степени вершин.
 *                Вектор будет очищен и заполнен. Индекс соответствует номеру вершины.
 * @param error_stream Поток для вывода сообщений об ошибках (на случай неквадратной матрицы).
 * @return true, если матрица корректна (квадратная) и степени вычислены, иначе false.
 *
 * Учитывает петли (m[i][i] != 0), добавляя 2 к степени соответствующей вершины за каждую петлю.
 * Для остальных ребер (m[i][j] != 0, i != j) добавляет 1 к степени вершины i.
 */
bool CalculateVertexDegrees(const std::vector<std::vector<int>>& adjacency_matrix,
	std::vector<int>& degrees,
	std::ostream& error_stream);

/**
 * @brief Выводит степени вершин графа в поток.
 * @param degrees Константная ссылка на вектор степеней вершин.
 * @param output_stream Поток для вывода результата.
 *
 * Выводит степени в формате "Вершина <i>: <степень>".
 */
void PrintVertexDegrees(const std::vector<int>& degrees, std::ostream& output_stream);


#endif 