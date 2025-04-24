/* Задание №1: Генератор паролей разной сложности */

#ifndef PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_
#define PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_

#include <string>   // Для std::string
#include <iostream> // Для std::istream, std::ostream
#include <random>   // Для генератора случайных чисел (std::mt19937)

// using namespace std; // ЗАПРЕЩЕНО в .h

/**
 * @brief Перечисление уровней сложности пароля.
 *
 * Определяет возможные варианты сложности, запрашиваемые у пользователя.
 */
enum class PasswordComplexity {
	EASY = 1, // Начинаем с 1 для удобства пользователя
	MEDIUM = 2,
	HARD = 3,
	INVALID // Для обозначения некорректного выбора
};

/**
 * @brief Проверяет корректность числового выбора уровня сложности.
 * @param choice Число, введенное пользователем.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return true, если выбор корректен (1, 2 или 3), иначе false.
 *
 * Используется для валидации ввода пользователя перед преобразованием в PasswordComplexity.
 */
bool ValidateComplexityChoice(int choice, std::ostream& error_stream);

/**
 * @brief Запрашивает у пользователя уровень сложности пароля.
 * @param input_stream Поток ввода (например, std::cin).
 * @param output_stream Поток вывода для сообщений пользователю (например, std::cout).
 * @param error_stream Поток вывода для сообщений об ошибках (например, std::cerr).
 * @return Выбранный пользователем уровень сложности (PasswordComplexity).
 *         В случае ошибки ввода или невалидного выбора возвращает PasswordComplexity::INVALID.
 *
 * Функция будет повторять запрос, пока пользователь не введет корректное число (1, 2 или 3).
 */
PasswordComplexity GetComplexityChoice(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream);

/**
 * @brief Генерирует случайный пароль указанной сложности.
 * @param level Уровень сложности пароля (EASY, MEDIUM, HARD).
 * @param random_engine Ссылка на инициализированный генератор псевдослучайных чисел (например, std::mt19937).
 *                      Передается по ссылке, чтобы избежать повторной инициализации и обеспечить лучшую случайность.
 * @param error_stream Поток для вывода сообщений об ошибках.
 * @return Сгенерированный пароль в виде строки. В случае ошибки (например, невалидный уровень)
 *         возвращает пустую строку и выводит сообщение в error_stream.
 *
 * Генерирует пароль на основе предопределенных наборов символов и длины для каждого уровня сложности.
 */
std::string GeneratePassword(PasswordComplexity level, std::mt19937& random_engine, std::ostream& error_stream);

/**
 * @brief Запрашивает у пользователя, хочет ли он сгенерировать еще один пароль.
 * @param input_stream Поток ввода.
 * @param output_stream Поток вывода.
 * @param error_stream Поток ошибок.
 * @return true, если пользователь хочет продолжить (ввел 'y' или 'д'), иначе false.
 *
 * Обеспечивает цикл повторной генерации паролей.
 */
bool AskToContinue(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream);


#endif // PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_