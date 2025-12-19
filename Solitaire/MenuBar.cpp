#include "MenuBar.h"
#include <iostream>

MenuBar::MenuBar() {
    background.setFillColor(sf::Color::White);

    sf::Vector2f buttonSize(120, 30);

    hintButton.setSize(buttonSize);
    hintButton.setFillColor(sf::Color::White);
    hintButton.setOutlineColor(sf::Color(200, 200, 200));
    hintButton.setOutlineThickness(2);

    font = std::make_shared<sf::Font>();
}

bool MenuBar::loadFont(const std::string& fontPath) {
    if (!font->openFromFile(fontPath)) {
        std::cerr << "Ошибка загрузки шрифта для меню: " << fontPath << std::endl;
        return false;
    }
    return true;
}

void MenuBar::setPosition(const sf::Vector2f& position) {
    background.setPosition(position);

    float panelWidth = background.getSize().x;
    float buttonWidth = hintButton.getSize().x;
    float x = position.x + (panelWidth - buttonWidth) / 2.0f;
    float y = position.y + 2.0f;

    hintButton.setPosition({ x, y });

    if (font) {
        if (!hintText.has_value()) {
            hintText.emplace(*font);
        }
        hintText->setString("Подсказка");
        hintText->setCharacterSize(16);
        hintText->setFillColor(sf::Color::Black);

        sf::FloatRect textBounds = hintText->getLocalBounds();
        float textX = x + (buttonWidth - textBounds.size.x) / 2.0f;
        float textY = y + (30 - textBounds.size.y) / 2.0f - 2.0f;
        hintText->setPosition({ textX, textY });
    }
}

void MenuBar::setSize(const sf::Vector2f& size) {
    background.setSize(size);
}

void MenuBar::setOnHintCallback(const std::function<void()>& callback) {
    onHintCallback = callback;
}

void MenuBar::handleClick(const sf::Vector2f& mousePos) {
    if (hintButton.getGlobalBounds().contains(mousePos) && onHintCallback) {
        hintButton.setFillColor(sf::Color(240, 240, 240));

        onHintCallback();

        hintButton.setFillColor(sf::Color::White);
    }
}

void MenuBar::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(hintButton);

    if (hintText.has_value()) {
        window.draw(*hintText);
    }
}