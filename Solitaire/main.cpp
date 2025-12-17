#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <clocale>
#include "Solitaire.h"
#include "windows.h"

int main() {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::locale::global(std::locale("Russian_Russia.1251"));

    try {
        Solitaire app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Îøèáêà: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}