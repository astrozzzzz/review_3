/* Продолжение задания №1 */

#include "graph.h"
#include <fstream>
using namespace std;

/**
 * Проверяет корректность входных данных.
 * @param file_name Имя файла с матрицей смежности.
 * @return true, если данные корректны, иначе false.
 */
bool CheckInputData(const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл." << endl;
        return false;
    }
    int n;
    if (!(file >> n) || n <= 0) {
        cerr << "Ошибка: некорректное количество вершин." << endl;
        file.close();
        return false;
    }
    int value;
    int count = 0;
    while (file >> value) {
        if (value < 0) {
            cerr << "Ошибка: элементы матрицы должны быть неотрицательными." << endl;
            file.close();
            return false;
        }
        ++count;
    }
    file.close();
    if (count != n * n) {
        cerr << "Ошибка: размер матрицы не соответствует количеству вершин." << endl;
        return false;
    }
    return true;
}

/**
 * Считывает матрицу смежности из файла.
 * @param file_name Имя файла с матрицей смежности.
 * @param matrix Матрица смежности (выходной параметр).
 * @return true, если успешно считано, иначе false.
 */
bool ReadAdjacencyMatrix(const string& file_name, vector<vector<int>>& matrix) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл." << endl;
        return false;
    }
    int n;
    file >> n;
    matrix.resize(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(file >> matrix[i][j])) {
                cerr << "Ошибка: некорректные данные в матрице." << endl;
                file.close();
                return false;
            }
        }
    }
    file.close();
    return true;
}

/**
 * Вычисляет степени вершин графа.
 * @param matrix Матрица смежности.
 * @return Вектор степеней вершин.
 */
vector<int> CalculateDegrees(const vector<vector<int>>& matrix) {
    int n = static_cast<int>(matrix.size());
    vector<int> degrees(n, 0);
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                // Петля учитывается дважды
                degree += 2 * matrix[i][j];
            } else {
                degree += matrix[i][j];
            }
        }
        degrees[i] = degree;
    }
    return degrees;
}

/**
 * Выводит степени вершин.
 * @param degrees Вектор степеней вершин.
 */
void PrintDegrees(const vector<int>& degrees) {
    // Выводим степени вершин по порядку
    for (size_t i = 0; i < degrees.size(); ++i) {
        cout << "Вершина " << (i + 1) << ": степень = " << degrees[i] << endl;
    }
}