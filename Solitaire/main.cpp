#include <iostream>
#include "Solitaire.h"
#include "windows.h"

int main() {
    SetConsoleOutputCP(1251);
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