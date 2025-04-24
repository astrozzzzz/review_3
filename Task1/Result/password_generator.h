/* Продолжение задания №1 */
#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H

#include <string>
#include <random>
#include <chrono>
#include <iostream>

/**
 * @brief Проверяет корректность введенного уровня сложности
 * @param level Уровень сложности (1-3)
 * @return true если уровень корректен, false если нет
 */
bool ValidateLevel(int level);

/**
 * @brief Генерирует пароль заданной сложности
 * @param level Уровень сложности (1-3)
 * @return Сгенерированный пароль
 */
std::string GeneratePassword(int level);

#endif