/* Продолжение задания №1 */

#include "password_generator.h"
#include <iostream>
#include <limits>

using namespace std;

/**
 * Точка входа в программу.
 */
int main() {
    setlocale(LC_ALL, "Russian");
    // Символы для разных уровней сложности
    const string charset_easy = "abcdefghijklmnopqrstuvwxyz";
    const string charset_medium = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const string charset_hard = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:,.<>?/";

    // Длины паролей для разных уровней
    const int length_easy = 6;
    const int length_medium = 10;
    const int length_hard = 16;

    cout << "Выберите уровень сложности пароля:\n";
    cout << "1 - Лёгкий (только строчные буквы, длина " << length_easy << ")\n";
    cout << "2 - Средний (буквы и цифры, длина " << length_medium << ")\n";
    cout << "3 - Сложный (буквы, цифры, символы, длина " << length_hard << ")\n";
    cout << "Введите номер уровня: ";

    string level;
    cin >> level;

    // Проверка корректности ввода
    if (!IsValidLevel(level)) {
        cerr << "Ошибка: выбран некорректный уровень сложности.\n";
        return 1;
    }

    string password;
    if (level == "1") {
        password = GeneratePassword(length_easy, charset_easy);
    } else if (level == "2") {
        password = GeneratePassword(length_medium, charset_medium);
    } else {
        password = GeneratePassword(length_hard, charset_hard);
    }

    cout << "Ваш сгенерированный пароль: " << password << endl;

    return 0;
}