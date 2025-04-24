/* Продолжение задания №1: Реализация функций генератора паролей */

#include "password_generator.h" // Сначала связанный заголовок

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <limits>   // Для numeric_limits
#include <algorithm> // Для std::shuffle (хотя здесь не используется, но полезно знать)
#include <cctype>   // Для isdigit, isspace, tolower

// Используем пространство имен std в .cpp файлах
using namespace std;

// --- Вспомогательные константы ---
// Определяем наборы символов один раз, чтобы не создавать их постоянно
const string kLowercaseChars = "abcdefghijklmnopqrstuvwxyz";
const string kUppercaseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string kDigitChars = "0123456789";
const string kSymbolChars = "!@#$%^&*()_+-=[]{};':\",./<>?";

// --- Вспомогательная функция ---
/**
 * @brief Очищает буфер потока ввода до следующего символа новой строки.
 * @param input_stream Поток ввода, который нужно очистить.
 *
 * Используется для восстановления потока после ошибки ввода.
 * Зачем? Чтобы некорректный ввод не мешал последующим операциям чтения.
 */
void ClearInputStream(istream& input_stream) {
	input_stream.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Реализация функций из .h ---

/**
 * @brief Реализация функции проверки выбора сложности. См. password_generator.h
 */
bool ValidateComplexityChoice(int choice, ostream& error_stream) {
	if (choice >= static_cast<int>(PasswordComplexity::EASY) &&
		choice <= static_cast<int>(PasswordComplexity::HARD)) { // Фигурные скобки на той же строке
		return true;
	}
	else {
		error_stream << "Ошибка: Неверный выбор. Введите число от "
			<< static_cast<int>(PasswordComplexity::EASY) << " до "
			<< static_cast<int>(PasswordComplexity::HARD) << "." << endl;
		return false;
	}
}

/**
 * @brief Реализация функции получения выбора сложности. См. password_generator.h
 */
PasswordComplexity GetComplexityChoice(istream& input_stream, ostream& output_stream, ostream& error_stream) {
	int choice = 0;
	while (true) { // Цикл для повторного запроса при неверном вводе
		output_stream << "Выберите уровень сложности пароля:" << endl;
		output_stream << static_cast<int>(PasswordComplexity::EASY) << " - Легкий (8 символов, буквы нижнего регистра + цифры)" << endl;
		output_stream << static_cast<int>(PasswordComplexity::MEDIUM) << " - Средний (12 символов, буквы нижнего и верхнего регистра + цифры)" << endl;
		output_stream << static_cast<int>(PasswordComplexity::HARD) << " - Сложный (16 символов, все типы символов)" << endl;
		output_stream << "Ваш выбор: ";

		// Пытаемся прочитать число
		if (!(input_stream >> choice)) {
			error_stream << "Ошибка: Введено не число. Пожалуйста, попробуйте снова." << endl;
			input_stream.clear(); // Сбрасываем флаги ошибок
			ClearInputStream(input_stream); // Очищаем буфер
			continue; // Переходим к следующей итерации цикла
		}

		// Очищаем остаток строки после числа
		ClearInputStream(input_stream);

		// Проверяем корректность введенного числа
		// Зачем? Чтобы убедиться, что пользователь выбрал один из предложенных уровней.
		if (ValidateComplexityChoice(choice, error_stream)) {
			return static_cast<PasswordComplexity>(choice); // Возвращаем валидный выбор
		}
		// Если выбор невалиден, ValidateComplexityChoice уже вывела ошибку, цикл продолжится
	}
}

/**
 * @brief Реализация функции генерации пароля. См. password_generator.h
 */
string GeneratePassword(PasswordComplexity level, mt19937& random_engine, ostream& error_stream) {
	string allowed_chars;
	int length = 0;

	// Определяем параметры генерации в зависимости от уровня сложности
	// Зачем? Чтобы установить правильный набор символов и длину для генератора.
	switch (level) { // Фигурные скобки на той же строке
	case PasswordComplexity::EASY:
		allowed_chars = kLowercaseChars + kDigitChars;
		length = 8;
		break; // Не забываем break
	case PasswordComplexity::MEDIUM:
		allowed_chars = kLowercaseChars + kUppercaseChars + kDigitChars;
		length = 12;
		break;
	case PasswordComplexity::HARD:
		allowed_chars = kLowercaseChars + kUppercaseChars + kDigitChars + kSymbolChars;
		length = 16;
		break;
	default: // Обрабатываем невалидный уровень сложности
		error_stream << "Ошибка: Передан неверный уровень сложности для генерации пароля." << endl;
		return ""; // Возвращаем пустую строку в случае ошибки
	}

	if (allowed_chars.empty() || length <= 0) { // Дополнительная проверка на всякий случай
		error_stream << "Критическая ошибка: Не удалось определить параметры для генерации пароля." << endl;
		return "";
	}

	string password = "";
	password.reserve(length); // Выделяем память заранее для небольшой оптимизации

	// Используем распределение для генерации случайных индексов в строке allowed_chars
	uniform_int_distribution<size_t> distribution(0, allowed_chars.length() - 1);

	// Генерируем пароль нужной длины
	// Зачем? Чтобы создать строку пароля, выбирая случайные символы из разрешенного набора.
	for (int i = 0; i < length; ++i) {
		size_t random_index = distribution(random_engine);
		password += allowed_chars[random_index];
	}

	return password;
}

/**
 * @brief Реализация функции запроса продолжения. См. password_generator.h
 */
bool AskToContinue(istream& input_stream, ostream& output_stream, ostream& error_stream) {
	string response;
	while (true) { // Фигурные скобки на той же строке
		output_stream << "Сгенерировать еще один пароль? (y/n или д/н): ";
		if (!getline(input_stream, response)) {
			error_stream << "Ошибка чтения ответа." << endl;
			return false; // Выходим при ошибке потока
		}

		// Приводим ответ к нижнему регистру для упрощения сравнения
		string lower_response;
		// Используем auto для итератора с комментарием
		for (/*char*/ auto c : response) { // c имеет тип char
			lower_response += tolower(static_cast<unsigned char>(c));
		}

		// Удаляем пробелы для простоты проверки
		string trimmed_response;
		// Используем auto для итератора с комментарием
		for (/*char*/ auto c : lower_response) { // c имеет тип char
			if (!isspace(static_cast<unsigned char>(c))) {
				trimmed_response += c;
			}
		}

		if (trimmed_response == "y" || trimmed_response == "д" || trimmed_response == "yes" || trimmed_response == "да") {
			return true;
		}
		else if (trimmed_response == "n" || trimmed_response == "н" || trimmed_response == "no" || trimmed_response == "нет") {
			return false;
		}
		else {
			error_stream << "Неверный ввод. Пожалуйста, введите 'y'/'д' или 'n'/'н'." << endl;
			// Повторяем запрос
		}
	}
}