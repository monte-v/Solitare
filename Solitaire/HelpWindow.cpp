#include "HelpWindow.h"
#include <iostream>

HelpWindow::HelpWindow() : isOpenFlag(false) {
    window.create(sf::VideoMode({ 600, 500 }), "Правила игры Косынка", sf::Style::Titlebar | sf::Style::Close);
    window.setVisible(false);  

    if (!initialize()) {
        std::cerr << "Не удалось инициализировать правила игры" << std::endl;
        window.close();
    }  
}

bool HelpWindow::initialize() {
    if (!loadResources()) {
        return false;
    }
    return true;
}

bool HelpWindow::loadResources() {
    if (!loadIcon("assets/icon.png")) {
        std::cerr << "Используется стандартная иконка окна" << std::endl;
        return false;
    }
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Ошибка загрузки шрифта для окна помощи" << std::endl;
        return false;
    }
    initializeText();
    return true;
}

bool HelpWindow::loadIcon(std::string path) {
    sf::Image icon;

    if (icon.loadFromFile(path)) {
        window.setIcon(icon);
        return true;
    }

    return false;
}

void HelpWindow::initializeText() {
    helpText = {
        "ПРАВИЛА ИГРЫ КОСЫНКА",
        "",
        "Цель игры:",
        "Переместить все карты в 4 стопки-домика (Foundation),",
        "расположив их по мастям в порядке от туза до короля.",
        "",
        "Игровое поле:",
        "• 7 стопок (Tableau) - основное поле для перемещения карт",
        "• Запас (Stock) - закрытая колода",
        "• Сброс (Waste) - открытые карты из Stock",
        "• 4 домика (Foundation) - для собранных мастей",
        "",
        "Основные правила:",
        "1. В Tableau карты кладутся по убыванию, чередуя цвета",
        "2. В Foundation карты кладутся по возрастанию одной масти",
        "3. Из Waste можно брать только верхнюю карту",
        "4. На Stock можно кликнуть чтобы взять новые карты",
        "",
        "Управление:",
        "• ЛКМ - взять/переместить карты",
        "• По Stock - взять новые карты",
        "• Меню: Новая игра, Помощь"
    };
}

void HelpWindow::open() {
    isOpenFlag = true;
    window.setVisible(true);
}

void HelpWindow::close() {
    isOpenFlag = false;
    window.setVisible(false);
}

void HelpWindow::processEvents() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (!event) continue;

        if (event->is<sf::Event::Closed>()) {
            close();
        }
    }
}

void HelpWindow::render() {
    if (!isOpenFlag) return;

    window.clear(sf::Color(240, 240, 240));

    sf::Text title(font);
    title.setFont(font); 
    title.setString("Правила игры Косынка");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::Blue);
    title.setPosition({ 50.0f, 20.0f });  
    window.draw(title);

    float y = 70.0f;
    for (const auto& line : helpText) {
        sf::Text text(font);
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setPosition({ 50.0f, y });
        window.draw(text);
        y += 25.0f;
    }

    window.display();
}