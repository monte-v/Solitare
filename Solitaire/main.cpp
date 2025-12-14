#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <clocale>
#include "Solitaire.h"
#include "windows.h"

int main() {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    std::cout << "=== ПАСЬЯНС КОСЫНКА ===" << std::endl;
    std::cout << "Запуск приложения..." << std::endl;
    try {
        Solitaire app;
        std::cout << "Окно создано. Запуск главного цикла..." << std::endl;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}