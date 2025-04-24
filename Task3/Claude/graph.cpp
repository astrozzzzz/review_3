/*
 * Продолжение задания №Graf1
 * Реализация методов для работы с графом
 */

#include "graph.h"
#include <fstream>
#include <iostream>

using namespace std;

Graph::Graph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Невозможно открыть файл " << filename << endl;
        return;
    }

    file >> vertexCount;
    adjacencyMatrix.resize(vertexCount, vector<int>(vertexCount));

    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            file >> adjacencyMatrix[i][j];
        }
    }

    file.close();
}

bool Graph::validateInput() const {
    // Проверка размерности матрицы
    if (vertexCount <= 0) {
        cerr << "Ошибка: Некорректное количество вершин" << endl;
        return false;
    }

    // Проверка симметричности матрицы (для неориентированного графа)
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
                cerr << "Ошибка: Матрица смежности не симметрична" << endl;
                return false;
            }
        }
    }

    // Проверка значений в матрице
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (adjacencyMatrix[i][j] < 0) {
                cerr << "Ошибка: Отрицательные значения в матрице смежности" << endl;
                return false;
            }
        }
    }

    return true;
}

vector<int> Graph::calculateVertexDegrees() const {
    vector<int> degrees(vertexCount, 0);
    
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            degrees[i] += adjacencyMatrix[i][j];
        }
        // Учитываем петли дважды
        if (adjacencyMatrix[i][i] > 0) {
            degrees[i] += adjacencyMatrix[i][i];
        }
    }
    
    return degrees;
}