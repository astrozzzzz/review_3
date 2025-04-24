/* Продолжение задания №1 */
#include "password_generator.h"

using namespace std;


bool ValidateLevel(int level) {
    if (level < 1 || level > 3) {
        cerr << "Ошибка: уровень сложности должен быть от 1 до 3" << endl;
        return false;
    }
    return true;
}

string GeneratePassword(int level) {
    const string lowercase = "abcdefghijklmnopqrstuvwxyz";
    const string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string digits = "0123456789";
    const string special = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    int length;
    switch (level) {
        case 1: length = 8; break;
        case 2: length = 12; break;
        case 3: length = 16; break;
        default: return "";
    }

    auto seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

    string password = "";
    string allowed_chars = "";

    // Формируем набор разрешенных символов в зависимости от уровня
    if (level >= 1) {
        allowed_chars += lowercase + uppercase + digits;
    }
    if (level >= 2) {
        allowed_chars += special;
    }

    uniform_int_distribution<int> distribution(0, allowed_chars.length() - 1);
    
    for (int i = 0; i < length; ++i) {
        password += allowed_chars[distribution(generator)];
    }

    if (level == 3) {
        password[0] = lowercase[distribution(generator) % lowercase.length()];
        password[1] = uppercase[distribution(generator) % uppercase.length()];
        password[2] = digits[distribution(generator) % digits.length()];
        password[3] = special[distribution(generator) % special.length()];
    }

    return password;
}