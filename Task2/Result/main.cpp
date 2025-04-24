/**
* CalcTree10. � ��������� ����� � ������ FN1 ���� �������������� ��������� � ���������
* �����. � ��������� ����� �������������� ��������: ��������(+), ���������(-), ���������(*),
* ������� ������(/), ������� �� �������(%), ���������� � �������(^), � ��� �� ����� ����� ��
* ���������� [1; 30] � ���������� x. ��� �������� ���������� � ������� ���������� �������
* ��������������� ����� �����. ��������� ������ ���������. ����� ����� ��������� �������� ��������� ���
* �������� �������� ���������� x � �������� ��������� � ��������� ���� � ������ FN2.
* ������������ ������, ������� ��� ���������� ���� x*A �� A*x, ��� A - ��������� ���������, � x
* - ����������. ������������ ������ ����� �������������� � ���� FN2 ��������� �������������
* ������, � ������� ������ �������� �� ���. ������ ������� ������ �������� � 4-� �������� �
* ����������� ������������ �� ������� ����. ��� ������� ���������� ������ ������ ����������
* �������� ������ ������ ��������� �� �������: �������� ����������� ���������� ����������� ��
* ������� ����� �������. ����� �������, ��������� 2+3+4+5, ��������, ������ ������������ ��� ((2+3)+4)+5, �
* �� ����� ������������ ��� (2+3)+(4+5) ��� 2+(3+(4+5)). ���������� ����
* ����������, ������� �������������, ����������� ���� int.
*/

#include "expression_tree.h" 

using namespace std;


const string kInputFilename = "FN1.txt"; 
const string kOutputFilename = "FN2.txt"; 

/**
 * @brief �������� ������� ���������.
 * @param argc ���������� ���������� ��������� ������ (�� ������������).
 * @param argv ������ ���������� ��������� ������ (�� ������������).
 * @return 0 � ������ ��������� ����������, 1 � ������ ������.
 *
 * ��������� ������ ����: ������ ���������, ���� x, �������, ����������,
 * ������ ����������, �������������� ������, ������ ������ � ����.
 */
int main(int argc, char* argv[]) { 
	setlocale(LC_ALL, "Russian");
	string expression = "";
	int x_value = 1;
	Node* expression_tree = nullptr; 

	cout << "--- ����������� ������ ��������� (CalcTree10) ---" << endl;

	cout << "������ ��������� �� ����� '" << kInputFilename << "'..." << endl;
	if (!ReadExpressionFromFile(kInputFilename, expression, cerr)) {
		return 1; 
	}
	cout << "��������� ���������: " << expression << endl;

	if (!ReadXValue(cin, cout, cerr, x_value)) {
		return 1;
	}
	cout << "������� �������� x = " << x_value << endl;

	cout << "���������� ������ ���������..." << endl;
	expression_tree = ParseExpression(expression, cerr);
	if (!expression_tree) {
		cerr << "�� ������� ��������� ������ ���������." << endl;
		return 1;
	}
	cout << "������ ��������� �������." << endl;

	cout << "���������� �������� ��������� ��� x = " << x_value << "..." << endl;
	bool evaluation_success = true; 
	int result = EvaluateTree(expression_tree, x_value, cerr, evaluation_success);

	ofstream output_file(kOutputFilename);
	if (!output_file.is_open()) {
		cerr << "������: �� ������� ������� �������� ���� '" << kOutputFilename << "' ��� ������." << endl;
		DeleteTree(expression_tree); 
		return 1;
	}
	cout << "������ ����������� � ���� '" << kOutputFilename << "'..." << endl;

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
	}

	cout << "�������������� ������ (x*A -> A*x)..." << endl;
	TransformTree(expression_tree);
	cout << "�������������� ���������." << endl;

	output_file << "\n--- ��������������� ������ ��������� ---" << endl;
	PrintTree(expression_tree, output_file); 
	output_file << "--- ����� ������ ---" << endl;
	cout << "��������������� ������ �������� � ����." << endl;

	cout << "������� ������..." << endl;
	DeleteTree(expression_tree); 
	expression_tree = nullptr; 

	output_file.close();
	cout << "������ ��������� ������� ���������." << endl;

	return 0;
}