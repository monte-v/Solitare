#include "WinScreen.h"
#include <iostream>

WinScreen::WinScreen() : visible(false), font(nullptr) {
    background.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setFillColor(sf::Color(46, 139, 87));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(3.0f);

    font = std::make_shared<sf::Font>();
}

bool WinScreen::loadFont(const std::string& fontPath) {
    if (!font->openFromFile(fontPath)) {
        std::cerr << "Ошибка загрузки шрифта для WinScreen: " << fontPath << std::endl;
        font = nullptr;
        return false;
    }

    if (!winText.has_value()) {
        winText.emplace(*font);
    }
    winText->setString("ПОБЕДА!");
    winText->setCharacterSize(60);
    winText->setFillColor(sf::Color::White);
    winText->setStyle(sf::Text::Bold);

    if (!timeText.has_value()) {
        timeText.emplace(*font);
    }
    timeText->setCharacterSize(36);
    timeText->setFillColor(sf::Color::White);

    if (!clickText.has_value()) {
        clickText.emplace(*font);
    }
    clickText->setString("Нажмите для новой игры");
    clickText->setCharacterSize(24);
    clickText->setFillColor(sf::Color::Yellow);

    return true;
}

void WinScreen::show(const std::string& time) {
    visible = true;
    timeText->setString("Ваше время: " + time);

    setPosition(background.getPosition().x, background.getPosition().y);
}

void WinScreen::hide() {
    visible = false;
}

void WinScreen::setPosition(float x, float y) {
    background.setPosition({ x, y });

    sf::Vector2f panelSize = panel.getSize();
    sf::Vector2f bgSize = background.getSize();

    float panelX = x + (bgSize.x - panelSize.x) / 2;
    float panelY = y + (bgSize.y - panelSize.y) / 2;
    panel.setPosition({ panelX, panelY });

    sf::Vector2f panelPos = panel.getPosition();
    float panelLeft = panelPos.x;
    float panelTop = panelPos.y;
    float panelWidth = panelSize.x;

    sf::FloatRect winBounds = winText->getLocalBounds();
    sf::FloatRect timeBounds = timeText->getLocalBounds();
    sf::FloatRect clickBounds = clickText->getLocalBounds();

    float winTextWidth = winBounds.size.x;
    float timeTextWidth = timeBounds.size.x;
    float clickTextWidth = clickBounds.size.x;

    winText->setPosition({ panelLeft + (panelWidth - winTextWidth) / 2, panelTop + 30 });

    timeText->setPosition({ panelLeft + (panelWidth - timeTextWidth) / 2, panelTop + 120 });

    clickText->setPosition({ panelLeft + (panelWidth - clickTextWidth) / 2, panelTop + 200 });
}

void WinScreen::setSize(float width, float height) {
    background.setSize({ width, height });
    panel.setSize({ width * 0.7f, height * 0.5f });

    setPosition(background.getPosition().x, background.getPosition().y);
}

bool WinScreen::handleClick(float mouseX, float mouseY) {
    if (!visible) return false;

    sf::Vector2f bgPos = background.getPosition();
    sf::Vector2f bgSize = background.getSize();

    sf::FloatRect bgRect({ bgPos.x, bgPos.y }, { bgSize.x, bgSize.y });

    if (bgRect.contains({ mouseX, mouseY })) {
        hide();
        return true;
    }

    return false;
}

void WinScreen::draw(sf::RenderWindow& window) const {
    if (!visible) return;

    window.draw(background);
    window.draw(panel);
    window.draw(*winText);
    window.draw(*timeText);
    window.draw(*clickText);
}

