/**
 * Graf1. ���� �������� ������������������ ����� � ��������� ����� � ������ FileName. � ����
 * ������� ���������. ������ ������ ����� �������� ���������� ������ ����� (n), � ���������
 * n ����� �������� ������� ��������� (m), m[i][j]=0, ���� ����� ����� ��������� i � j ��
 * ����������. ���������� ������� ��� ������ ������� �����. ������� ������� ������,
 * ���������� ��� ������� ����������� ������� ������. ���� � ����� ������� �����, �� ������ ����� � �������
 * ������� ����������� ������.
*/

#include "graph_utils.h"

using namespace std;

const string kInputFilename = "graph_input.txt";

int main(int argc, char* argv[]) {
	string filename = kInputFilename;

	if (argc > 2) {
		cerr << "������: ������� ����� ���������� ��������� ������." << endl;
		cerr << "�������������: " << argv[0] << " [���_��������_�����]" << endl;
		return 1;
	}
	if (argc == 2) {
		filename = argv[1];
	}

	cout << "--- ����������� �������� ������ ����� ---" << endl;
	cout << "������ ����� �� �����: " << filename << endl;

	vector<vector<int>> adjacency_matrix;
	vector<int> vertex_degrees;

	if (!ReadAdjacencyMatrix(filename, adjacency_matrix, cerr)) {
		return 1;
	}
	cout << "������� ��������� ������� ��������� (" << adjacency_matrix.size() << " ������)." << endl;

	if (!CalculateVertexDegrees(adjacency_matrix, vertex_degrees, cerr)) {
		return 1;
	}
	cout << "������� ������ ���������." << endl;

	PrintVertexDegrees(vertex_degrees, cout);

	cout << "������ ��������� ������� ���������." << endl;
	return 0;
}