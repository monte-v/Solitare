#include "MenuBar.h"
#include <iostream>

MenuBar::MenuBar() : onNewGameCallback(nullptr) {
    background.setFillColor(sf::Color::White);

    sf::Vector2f buttonSize(120, 30);

    newGameButton.setSize(buttonSize);
    newGameButton.setFillColor(sf::Color::White);
    newGameButton.setOutlineColor(sf::Color(200, 200, 200));
    newGameButton.setOutlineThickness(2);

    hintButton.setSize(buttonSize); 
    hintButton.setFillColor(sf::Color::White);
    hintButton.setOutlineColor(sf::Color(200, 200, 200));
    hintButton.setOutlineThickness(2);

    helpButton.setSize(buttonSize);
    helpButton.setFillColor(sf::Color::White);
    helpButton.setOutlineColor(sf::Color(200, 200, 200));
    helpButton.setOutlineThickness(2);

    font = std::make_shared<sf::Font>();
}

bool MenuBar::loadFont(const std::string& fontPath) {
    std::string path = fontPath;
    if (path.empty()) {
        path = "assets/fonts/arial.ttf";
    }

    if (!font->openFromFile(path)) {
        std::cerr << "Œ¯Ë·Í‡ Á‡„ÛÁÍË ¯ËÙÚ‡ ‰Îˇ ÏÂÌ˛: " << path << std::endl;
        return false;
    }
    return true;
}

void MenuBar::setPosition(const sf::Vector2f& position) {
    background.setPosition(position);

    float x = position.x + 10;
    float y = position.y + 2;
    float spacing = 10;

    newGameButton.setPosition({ x, y });
    helpButton.setPosition({ x + 120 + spacing, y });
    hintButton.setPosition({ x + 240 + spacing * 2, y }); // ÕŒ¬¿ﬂ  ÕŒœ ¿

    if (font) {
        if (!newGameText.has_value()) {
            newGameText.emplace(*font);
        }

        newGameText->setString("ÕÓ‚‡ˇ Ë„‡");
        newGameText->setCharacterSize(16);
        newGameText->setFillColor(sf::Color::Black);
        newGameText->setPosition({ x + 20, y + 6 });

        if (!helpText.has_value()) {
            helpText.emplace(*font);
        }
        helpText->setString("œÓÏÓ˘¸");
        helpText->setCharacterSize(16);
        helpText->setFillColor(sf::Color::Black);
        helpText->setPosition({ x + 120 + spacing + 30, y + 6 });

        if (!hintText.has_value()) { 
            hintText.emplace(*font);
        }
        hintText->setString("œÓ‰ÒÍ‡ÁÍ‡");
        hintText->setCharacterSize(16);
        hintText->setFillColor(sf::Color::Black);
        hintText->setPosition({ x + 240 + spacing * 2 + 20, y + 6 });
    }
}

void MenuBar::setSize(const sf::Vector2f& size) {
    background.setSize(size);
}

void MenuBar::setOnNewGameCallback(const std::function<void()>& callback) {
    onNewGameCallback = callback;
}

void MenuBar::setOnHelpCallback(const std::function<void()>& callback) {
    onHelpCallback = callback;
}

void MenuBar::setOnHintCallback(const std::function<void()>& callback) {
    onHintCallback = callback;
}

void MenuBar::handleClick(const sf::Vector2f& mousePos) {
    if (newGameButton.getGlobalBounds().contains(mousePos) && onNewGameCallback) {
        newGameButton.setFillColor(sf::Color(240, 240, 240));
        onNewGameCallback();
        newGameButton.setFillColor(sf::Color::White);
    }
    if (helpButton.getGlobalBounds().contains(mousePos) && onHelpCallback) {
        helpButton.setFillColor(sf::Color(240, 240, 240));
        onHelpCallback();
        helpButton.setFillColor(sf::Color::White);
    }
    if (hintButton.getGlobalBounds().contains(mousePos) && onHintCallback) { // ÕŒ¬€… Œ¡–¿¡Œ“◊» 
        hintButton.setFillColor(sf::Color(240, 240, 240));
        onHintCallback();
        hintButton.setFillColor(sf::Color::White);
    }
}

void MenuBar::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(newGameButton);
    window.draw(helpButton);
    window.draw(hintButton);

    if (newGameText) window.draw(*newGameText);
    if (helpText) window.draw(*helpText);
    if (hintText) window.draw(*hintText);
}