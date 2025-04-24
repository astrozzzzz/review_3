/* Продолжение задания №1 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

/**
 * Проверяет корректность входных данных.
 * @param file_name Имя файла с матрицей смежности.
 * @return true, если данные корректны, иначе false.
 */
bool CheckInputData(const std::string& file_name);

/**
 * Считывает матрицу смежности из файла.
 * @param file_name Имя файла с матрицей смежности.
 * @param matrix Матрица смежности (выходной параметр).
 * @return true, если успешно считано, иначе false.
 */
bool ReadAdjacencyMatrix(const std::string& file_name, std::vector<std::vector<int>>& matrix);

/**
 * Вычисляет степени вершин графа.
 * @param matrix Матрица смежности.
 * @return Вектор степеней вершин.
 */
std::vector<int> CalculateDegrees(const std::vector<std::vector<int>>& matrix);

/**
 * Выводит степени вершин.
 * @param degrees Вектор степеней вершин.
 */
void PrintDegrees(const std::vector<int>& degrees);