/* 
 * Задача CalcTree10
 * Главный файл программы
 */

#include "tree.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

/**
 * Проверяет корректность входных данных из файла
 * @param filename Имя входного файла
 * @param expression Строка для сохранения выражения
 * @return true если данные корректны
 */
bool validateInput(const string& filename, string& expression) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть входной файл" << endl;
        return false;
    }
    
    getline(file, expression);
    file.close();
    
    // Проверка на пустое выражение
    if (expression.empty()) {
        cerr << "Ошибка: пустое выражение" << endl;
        return false;
    }
    
    // Проверка на допустимые символы
    for (char c : expression) {
        if (!isdigit(c) && c != 'x' && c != '+' && c != '-' && c != '*' && 
            c != '/' && c != '%' && c != '^' && c != '(' && c != ')' && 
            !isspace(c)) {
            cerr << "Ошибка: недопустимый символ в выражении" << endl;
            return false;
        }
    }
    
    return true;
}

int main() {
    string expression;
    int x;
    
    // Чтение и проверка входных данных
    if (!validateInput("FN1", expression)) {
        return 1;
    }
    
    // Создание и обработка дерева выражений
    ExpressionTree tree;
    if (!tree.buildTree(expression)) {
        return 1;
    }
    
    // Ввод значения x
    cout << "Введите значение x (1-30): ";
    cin >> x;
    
    if (x < 1 || x > 30) {
        cerr << "Ошибка: значение x должно быть в диапазоне [1; 30]" << endl;
        return 1;
    }
    
    // Вычисление результата
    int result = tree.evaluate(x);
    
    // Открытие файла для записи результата
    ofstream outFile("FN2");
    if (!outFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл FN2 для записи" << endl;
        return 1;
    }
    
    // Запись результата
    outFile << "Результат вычисления: " << result << endl << endl;
    outFile.close();
    
    // Преобразование дерева
    tree.transformMultiplication();
    
    // Вывод преобразованного дерева
    tree.printTree("FN2");
    
    return 0;
}