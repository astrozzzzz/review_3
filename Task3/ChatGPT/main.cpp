/* Продолжение задания №1 */

#include "graph.h"
using namespace std;

int main() {
    string file_name;
    cout << "Введите имя файла с матрицей смежности: ";
    cin >> file_name;

    if (!CheckInputData(file_name)) {
        return 1;
    }

    vector<vector<int>> matrix;
    if (!ReadAdjacencyMatrix(file_name, matrix)) {
        return 1;
    }

    vector<int> degrees = CalculateDegrees(matrix);
    PrintDegrees(degrees);

    return 0;
}