#include "MenuBar.h"
#include <iostream>

MenuBar::MenuBar() : onNewGameCallback(nullptr) {
    background.setFillColor(sf::Color::White);

    newGameButton.setSize(sf::Vector2f(120, 30));
    newGameButton.setFillColor(sf::Color::White);
    newGameButton.setOutlineColor(sf::Color(200, 200, 200));
    newGameButton.setOutlineThickness(2);

    font = std::make_shared<sf::Font>();
}

bool MenuBar::loadFont(const std::string& fontPath) {
    std::string path = fontPath;

    if (!font->openFromFile(path)) {
        std::cerr << "Ошибка загрузки шрифта для меню: " << path << std::endl;
        return false;
    }
    return true;
}

void MenuBar::setPosition(const sf::Vector2f& position) {
    background.setPosition(position);
    newGameButton.setPosition({ position.x + 10, position.y + 2 });

    // Создаем или обновляем текст
    if (font) {
        // Если optional пустой, создаем новый текст
        if (!newGameText.has_value()) {
            newGameText.emplace(*font);
        }
        else {
            // Если уже существует, просто обновляем шрифт
            newGameText->setFont(*font);
        }
    }

    if (newGameText.has_value()) {
        newGameText->setString("Новая игра");
        newGameText->setCharacterSize(16);
        newGameText->setFillColor(sf::Color::Black);
        newGameText->setPosition({ position.x + 30, position.y + 6 });
    }

}

void MenuBar::setSize(const sf::Vector2f& size) {
    background.setSize(size);
}

void MenuBar::setOnNewGameCallback(const std::function<void()>& callback) {
    onNewGameCallback = callback;
}

void MenuBar::handleClick(const sf::Vector2f& mousePos) {
    if (newGameButton.getGlobalBounds().contains(mousePos) && onNewGameCallback) {
        // Визуальная обратная связь
        newGameButton.setFillColor(sf::Color(240, 240, 240));
        onNewGameCallback();

        // Возвращаем цвет через короткое время (можно сделать через таймер)
        // Для простоты вернем сразу
        newGameButton.setFillColor(sf::Color::White);
    }
}

void MenuBar::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(newGameButton);
    if (newGameText) {
        window.draw(*newGameText);
    }
}