/* 
 * Задача CalcTree10
 * Построение и обработка дерева арифметического выражения
 */

#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

using namespace std;

// Структура для узла дерева выражений
struct Node {
    string value;      // Значение узла (операция или число/переменная)
    Node* left;        // Левый потомок
    Node* right;       // Правый потомок
    
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
public:
    /**
     * Конструктор класса
     */
    ExpressionTree();
    
    /**
     * Деструктор класса
     */
    ~ExpressionTree();

    /**
     * Создает дерево из инфиксного выражения
     * @param expression Строка с инфиксным выражением
     * @return true если создание прошло успешно
     */
    bool buildTree(const string& expression);
    
    /**
     * Вычисляет значение выражения
     * @param x Значение переменной x
     * @return Результат вычисления
     */
    int evaluate(int x);
    
    /**
     * Преобразует поддеревья вида x*A в A*x
     */
    void transformMultiplication();
    
    /**
     * Выводит дерево в файл в многострочном формате
     * @param filename Имя файла для вывода
     */
    void printTree(const string& filename);

private:
    Node* root;
    
    /**
     * Очищает память, занятую деревом
     * @param node Корень поддерева для удаления
     */
    void clearTree(Node* node);
    
    /**
     * Проверяет приоритет операции
     * @param op Операция
     * @return Приоритет операции
     */
    int getPriority(char op);
    
    /**
     * Создает узел из токена
     * @param token Строковое представление токена
     * @return Указатель на созданный узел
     */
    Node* createNode(const string& token);
    
    /**
     * Разбивает выражение на токены
     * @param expression Входное выражение
     * @return Вектор токенов
     */
    vector<string> tokenize(const string& expression);
    
    /**
     * Вспомогательный метод для вычисления значения
     * @param node Текущий узел
     * @param x Значение переменной
     * @return Результат вычисления
     */
    int evaluateHelper(Node* node, int x);
    
    /**
     * Вспомогательный метод для преобразования умножения
     * @param node Текущий узел
     */
    void transformMultiplicationHelper(Node* node);
    
    /**
     * Вспомогательный метод для вывода дерева
     * @param node Текущий узел
     * @param level Уровень вложенности
     * @param file Указатель на файл
     */
    void printTreeHelper(Node* node, int level, FILE* file);
};

#endif