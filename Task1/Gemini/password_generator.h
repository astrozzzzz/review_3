/* ������� �1: ��������� ������� ������ ��������� */

#ifndef PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_
#define PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_

#include <string>   // ��� std::string
#include <iostream> // ��� std::istream, std::ostream
#include <random>   // ��� ���������� ��������� ����� (std::mt19937)

// using namespace std; // ��������� � .h

/**
 * @brief ������������ ������� ��������� ������.
 *
 * ���������� ��������� �������� ���������, ������������� � ������������.
 */
enum class PasswordComplexity {
	EASY = 1, // �������� � 1 ��� �������� ������������
	MEDIUM = 2,
	HARD = 3,
	INVALID // ��� ����������� ������������� ������
};

/**
 * @brief ��������� ������������ ��������� ������ ������ ���������.
 * @param choice �����, ��������� �������������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return true, ���� ����� ��������� (1, 2 ��� 3), ����� false.
 *
 * ������������ ��� ��������� ����� ������������ ����� ��������������� � PasswordComplexity.
 */
bool ValidateComplexityChoice(int choice, std::ostream& error_stream);

/**
 * @brief ����������� � ������������ ������� ��������� ������.
 * @param input_stream ����� ����� (��������, std::cin).
 * @param output_stream ����� ������ ��� ��������� ������������ (��������, std::cout).
 * @param error_stream ����� ������ ��� ��������� �� ������� (��������, std::cerr).
 * @return ��������� ������������� ������� ��������� (PasswordComplexity).
 *         � ������ ������ ����� ��� ����������� ������ ���������� PasswordComplexity::INVALID.
 *
 * ������� ����� ��������� ������, ���� ������������ �� ������ ���������� ����� (1, 2 ��� 3).
 */
PasswordComplexity GetComplexityChoice(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream);

/**
 * @brief ���������� ��������� ������ ��������� ���������.
 * @param level ������� ��������� ������ (EASY, MEDIUM, HARD).
 * @param random_engine ������ �� ������������������ ��������� ��������������� ����� (��������, std::mt19937).
 *                      ���������� �� ������, ����� �������� ��������� ������������� � ���������� ������ �����������.
 * @param error_stream ����� ��� ������ ��������� �� �������.
 * @return ��������������� ������ � ���� ������. � ������ ������ (��������, ���������� �������)
 *         ���������� ������ ������ � ������� ��������� � error_stream.
 *
 * ���������� ������ �� ������ ���������������� ������� �������� � ����� ��� ������� ������ ���������.
 */
std::string GeneratePassword(PasswordComplexity level, std::mt19937& random_engine, std::ostream& error_stream);

/**
 * @brief ����������� � ������������, ����� �� �� ������������� ��� ���� ������.
 * @param input_stream ����� �����.
 * @param output_stream ����� ������.
 * @param error_stream ����� ������.
 * @return true, ���� ������������ ����� ���������� (���� 'y' ��� '�'), ����� false.
 *
 * ������������ ���� ��������� ��������� �������.
 */
bool AskToContinue(std::istream& input_stream, std::ostream& output_stream, std::ostream& error_stream);


#endif // PASSWORD_GENERATOR_PASSWORD_GENERATOR_H_