/* ����������� ������� Graf1 */

#ifndef GRAPH_UTILS_GRAPH_UTILS_H_
#define GRAPH_UTILS_GRAPH_UTILS_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

/**
 * @brief ������ ������� ��������� ������������������ ����� �� �����.
 * @param filename ��� �������� �����.
 * @param adjacency_matrix ������ �� 2D ������, ���� ����� �������� �������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true � ������ ��������� ������ � ��������� ������� ��������� �����, ����� false.
 *
 * ������ ������ ����� ������ ��������� ���������� ������ n.
 * ��������� n ����� ������ ��������� �� n ����� �����, ����������� ���������.
 * ������� ��������� �������� �����, ������������ ������ n � ����������� �������.
 * �� ��������� �������� ������ ������� (����� ����, ��� ��� �����).
 */
bool ReadAdjacencyMatrix(const std::string& filename,
	std::vector<std::vector<int>>& adjacency_matrix,
	std::ostream& error_stream);

/**
 * @brief ��������� ������� ���� ������ ����� �� ��� ������� ���������.
 * @param adjacency_matrix ����������� ������ �� ������� ��������� �����.
 * @param degrees ������ �� ������, ���� ����� �������� ������� ������.
 *                ������ ����� ������ � ��������. ������ ������������� ������ �������.
 * @param error_stream ����� ��� ������ ��������� �� ������� (�� ������ ������������ �������).
 * @return true, ���� ������� ��������� (����������) � ������� ���������, ����� false.
 *
 * ��������� ����� (m[i][i] != 0), �������� 2 � ������� ��������������� ������� �� ������ �����.
 * ��� ��������� ����� (m[i][j] != 0, i != j) ��������� 1 � ������� ������� i.
 */
bool CalculateVertexDegrees(const std::vector<std::vector<int>>& adjacency_matrix,
	std::vector<int>& degrees,
	std::ostream& error_stream);

/**
 * @brief ������� ������� ������ ����� � �����.
 * @param degrees ����������� ������ �� ������ �������� ������.
 * @param output_stream ����� ��� ������ ����������.
 *
 * ������� ������� � ������� "������� <i>: <�������>".
 */
void PrintVertexDegrees(const std::vector<int>& degrees, std::ostream& output_stream);


#endif 