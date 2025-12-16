#include "StartMenu.h"
#include <iostream>

StartMenu::StartMenu() : visible(true) {
}

bool StartMenu::initialize(const std::string& fontPath) {
    // Загружаем шрифт
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Не удалось загрузить шрифт: " << fontPath << std::endl;
        return false;
    }

    // Создаем UI
    createUI();

    return true;
}

void StartMenu::createUI() {
    // Создаем кнопки
    startButton = Button("Начать игру", { 350.0f, 300.0f }, { 300.0f, 60.0f }, font);
    helpButton = Button("Помощь", { 350.0f, 380.0f }, { 300.0f, 60.0f }, font);
    exitButton = Button("Выход", { 350.0f, 460.0f }, { 300.0f, 60.0f }, font);

    // Создаем заголовок
    if (!title.has_value()) {
        title.emplace(font);
    }
    title->setString("ПАСЬЯНС КОСЫНКА");
    title->setCharacterSize(48);
    title->setFillColor(sf::Color::White);
    title->setPosition({ 290.0f, 150.0f });
}

void StartMenu::handleClick(const sf::Vector2f& mousePos) {
    if (!visible) return;

    if (startButton.contains(mousePos)) {
        startButton.click();
    }
    else if (helpButton.contains(mousePos)) {
        helpButton.click();
    }
    else if (exitButton.contains(mousePos)) {
        exitButton.click();
    }
}

void StartMenu::draw(sf::RenderWindow& window) const {
    if (!visible) return;

    // Полупрозрачный черный фон
    sf::RectangleShape overlay;
    overlay.setSize({ 1000.0f, 800.0f });
    overlay.setFillColor(sf::Color(255, 255, 255, 200));
    window.draw(overlay);

    // Темный прямоугольник меню
    sf::RectangleShape menuBg;
    menuBg.setSize({ 700.0f, 500.0f });
    menuBg.setPosition({ 150.0f, 150.0f });
    menuBg.setFillColor(sf::Color(30, 30, 30, 230));
    menuBg.setOutlineColor(sf::Color::White);
    menuBg.setOutlineThickness(2.0f);
    window.draw(menuBg);

    // Рисуем элементы
    if (title.has_value()) {
        window.draw(*title);
    }
    startButton.draw(window);
    helpButton.draw(window);
    exitButton.draw(window);
}