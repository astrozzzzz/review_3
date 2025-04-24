/* Продолжение задания №1 */

#pragma once

#include <iostream>
#include <string>

/**
 * Проверяет корректность выбора уровня сложности.
 * @param level Строка, введённая пользователем.
 * @return true, если уровень корректен, иначе false.
 */
bool IsValidLevel(const std::string& level);

/**
 * Генерирует пароль заданной длины и сложности.
 * @param length Длина пароля.
 * @param charset Набор символов для генерации.
 * @return Сгенерированный пароль.
 */
std::string GeneratePassword(int length, const std::string& charset);