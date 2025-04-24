#include "password_generator.h"
#include <iostream>

using namespace std;

int main() {
    int level;
    char continue_generation;

    do {
        cout << "Выберите уровень сложности пароля:" << endl;
        cout << "1 - Базовый (8 символов: буквы и цифры)" << endl;
        cout << "2 - Средний (12 символов: буквы, цифры и спец. символы)" << endl;
        cout << "3 - Сложный (16 символов: гарантированное включение всех типов символов)" << endl;
        cout << "Введите уровень (1-3): ";
        
        // Проверка корректности ввода
        if (!(cin >> level)) {
            cerr << "Ошибка: введите число" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (validateLevel(level)) {
            string password = generatePassword(level);
            cout << "Сгенерированный пароль: " << password << endl;
        }

        cout << "Хотите сгенерировать еще один пароль? (y/n): ";
        cin >> continue_generation;
        
    } while (continue_generation == 'y' || continue_generation == 'Y');

    return 0;
}