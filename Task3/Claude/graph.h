/*
 * Продолжение задания №Graf1
 * Определение степеней вершин неориентированного графа, заданного матрицей смежности
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

class Graph {
public:
    /**
     * Конструктор создает граф из файла
     * @param filename путь к файлу с матрицей смежности
     */
    explicit Graph(const string& filename);
    
    /**
     * Вычисляет степени всех вершин графа
     * @return вектор степеней вершин
     */
    vector<int> calculateVertexDegrees() const;
    
    /**
     * Проверяет корректность входных данных из файла
     * @return true если данные корректны, false иначе
     */
    bool validateInput() const;

private:
    int vertexCount;                    // Количество вершин
    vector<vector<int>> adjacencyMatrix;// Матрица смежности
};

#endif