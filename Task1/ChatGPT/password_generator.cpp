/* Продолжение задания №1 */

#include "password_generator.h"
#include <random>
#include <limits>

using namespace std;

/**
 * Проверяет корректность выбора уровня сложности.
 * @param level Строка, введённая пользователем.
 * @return true, если уровень корректен, иначе false.
 */
bool IsValidLevel(const string& level) {
    // Проверяем, что введён один из допустимых уровней
    return level == "1" || level == "2" || level == "3";
}

/**
 * Генерирует пароль заданной длины и сложности.
 * @param length Длина пароля.
 * @param charset Набор символов для генерации.
 * @return Сгенерированный пароль.
 */
string GeneratePassword(int length, const string& charset) {
    // Используем mt19937 для генерации случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, static_cast<int>(charset.size()) - 1);

    string password;
    for (int i = 0; i < length; ++i) {
        // auto используется только для итератора, но здесь тип int
        password += charset[distrib(gen)];
    }
    return password;
}