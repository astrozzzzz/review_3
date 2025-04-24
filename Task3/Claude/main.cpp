/*
 * Продолжение задания №Graf1
 * Главный файл программы
 */

#include "graph.h"
#include <iostream>

using namespace std;

int main() {
    string filename;
    cout << "Введите имя файла с матрицей смежности: ";
    cin >> filename;

    Graph graph(filename);

    if (!graph.validateInput()) {
        return 1;
    }

    vector<int> degrees = graph.calculateVertexDegrees();

    cout << "Степени вершин графа:" << endl;
    for (int i = 0; i < degrees.size(); i++) {
        cout << "Вершина " << (i + 1) << ": " << degrees[i] << endl;
    }

    return 0;
}