/* ������� CalcTree10: ����� ����� ��� ������ � ������� ��������� */

#include "expression_tree.h" // ��� ���������
#include <iostream>
#include <string>
#include <fstream> // ��� ��������� �����

// ���������� ������������ ���� std � .cpp ������
using namespace std;

// --- ��������� ��� ���� ������ ---
// �����? ����� ����� ������ ����� ������ � ����� �����.
const string kInputFilename = "FN1.txt"; // ��� �������� �����
const string kOutputFilename = "FN2.txt"; // ��� ��������� �����

/**
 * @brief �������� ������� ���������.
 * @param argc ���������� ���������� ��������� ������ (�� ������������).
 * @param argv ������ ���������� ��������� ������ (�� ������������).
 * @return 0 � ������ ��������� ����������, 1 � ������ ������.
 *
 * ��������� ������ ����: ������ ���������, ���� x, �������, ����������,
 * ������ ����������, �������������� ������, ������ ������ � ����.
 */
int main(int argc, char* argv[]) { // �������� ������ �� ��� �� ������
	setlocale(LC_ALL, "Russian");
	string expression;
	int x_value;
	Node* expression_tree = nullptr; // �������������� ��������� �� ������

	cout << "--- ����������� ������ ��������� (CalcTree10) ---" << endl;

	// 1. ������ ��������� �� ����� FN1
	cout << "������ ��������� �� ����� '" << kInputFilename << "'..." << endl;
	if (!ReadExpressionFromFile(kInputFilename, expression, cerr)) {
		// ��������� �� ������ ��� ��������
		return 1; // ��������� � �������
	}
	cout << "��������� ���������: " << expression << endl;

	// 2. ������ �������� x � ������������
	if (!ReadXValue(cin, cout, cerr, x_value)) {
		// ��������� �� ������ ��� ��������
		return 1;
	}
	cout << "������� �������� x = " << x_value << endl;

	// 3. ������� ��������� � ���������� ������
	cout << "���������� ������ ���������..." << endl;
	expression_tree = ParseExpression(expression, cerr);
	if (!expression_tree) {
		cerr << "�� ������� ��������� ������ ���������." << endl;
		// ������� �� �����, �.�. ParseExpression ��� ������ ��� ������
		return 1;
	}
	cout << "������ ��������� �������." << endl;

	// 4. ���������� �������� ���������
	cout << "���������� �������� ��������� ��� x = " << x_value << "..." << endl;
	bool evaluation_success = true; // ���� ��� ������������ ������ ����������
	int result = EvaluateTree(expression_tree, x_value, cerr, evaluation_success);

	// 5. �������� ��������� ����� FN2
	ofstream output_file(kOutputFilename);
	if (!output_file.is_open()) {
		cerr << "������: �� ������� ������� �������� ���� '" << kOutputFilename << "' ��� ������." << endl;
		DeleteTree(expression_tree); // ����������� ������ ����� �������
		return 1;
	}
	cout << "������ ����������� � ���� '" << kOutputFilename << "'..." << endl;


	// 6. ������ ���������� ���������� � FN2 (���� ���������� �������)
	if (evaluation_success) {
		output_file << "���������: " << expression << endl;
		output_file << "�������� x: " << x_value << endl;
		output_file << "��������� ����������: " << result << endl;
		cout << "��������� ����������: " << result << endl;
	}
	else {
		output_file << "���������: " << expression << endl;
		output_file << "�������� x: " << x_value << endl;
		output_file << "��������� ����������: ������ (��. �������)" << endl;
		cerr << "������ ��� ���������� ���������. ��������� �� ������� ��� �����." << endl;
		// �� ������� � �������, ��������� � ������� ������
	}

	// 7. �������������� ������
	cout << "�������������� ������ (x*A -> A*x)..." << endl;
	TransformTree(expression_tree);
	cout << "�������������� ���������." << endl;

	// 8. ������ ���������������� ������ � FN2
	output_file << "\n--- ��������������� ������ ��������� ---" << endl;
	PrintTree(expression_tree, output_file); // �������� � ����
	output_file << "--- ����� ������ ---" << endl;
	cout << "��������������� ������ �������� � ����." << endl;

	// 9. ������� ������ � �������� �����
	cout << "������� ������..." << endl;
	DeleteTree(expression_tree); // ����������� ����������� ������!
	expression_tree = nullptr; // ������� �������� �������� ��������� ����� delete

	output_file.close(); // ���� ��������� ����
	cout << "������ ��������� ������� ���������." << endl;

	return 0; // �������� ����������
}