#include "WinScreen.h"
#include <iostream>

WinScreen::WinScreen() : visible(false), font(nullptr) {
    background.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setFillColor(sf::Color(46, 139, 87));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(3.0f);
}

WinScreen::~WinScreen() {
    if (font) {
        delete font;
        font = nullptr;
    }
}

bool WinScreen::loadFont(const std::string& fontPath) {
    font = new sf::Font();
    if (!font->openFromFile(fontPath)) {
        std::cerr << "Ошибка загрузки шрифта для WinScreen: " << fontPath << std::endl;
        delete font;
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
    timeText->setString("Время: " + time);

    // Обновляем позиции текстов
    setPosition(background.getPosition().x, background.getPosition().y);
}

void WinScreen::hide() {
    visible = false;
}

void WinScreen::setPosition(float x, float y) {
    background.setPosition({ x, y });

    // Центрируем панель
    sf::Vector2f panelSize = panel.getSize();
    sf::Vector2f bgSize = background.getSize();

    float panelX = x + (bgSize.x - panelSize.x) / 2;
    float panelY = y + (bgSize.y - panelSize.y) / 2;
    panel.setPosition({ panelX, panelY });

    // Получаем позицию панели
    sf::Vector2f panelPos = panel.getPosition();
    float panelLeft = panelPos.x;
    float panelTop = panelPos.y;
    float panelWidth = panelSize.x;

    // Получаем ширину текстов через их границы
    sf::FloatRect winBounds = winText->getLocalBounds();
    sf::FloatRect timeBounds = timeText->getLocalBounds();
    sf::FloatRect clickBounds = clickText->getLocalBounds();

    // В SFML 3.0 используем size для получения ширины
    float winTextWidth = winBounds.size.x;
    float timeTextWidth = timeBounds.size.x;
    float clickTextWidth = clickBounds.size.x;

    // Центрируем winText
    winText->setPosition({ panelLeft + (panelWidth - winTextWidth) / 2, panelTop + 30 });

    // Центрируем timeText
    timeText->setPosition({ panelLeft + (panelWidth - timeTextWidth) / 2, panelTop + 120 });

    // Центрируем clickText
    clickText->setPosition({ panelLeft + (panelWidth - clickTextWidth) / 2, panelTop + 200 });
}

void WinScreen::setSize(float width, float height) {
    background.setSize({ width, height });
    panel.setSize({ width * 0.7f, height * 0.5f });

    // Обновляем позиции
    setPosition(background.getPosition().x, background.getPosition().y);
}

bool WinScreen::handleClick(float mouseX, float mouseY) {
    if (!visible) return false;

    // Проверяем клик в любом месте
    sf::Vector2f bgPos = background.getPosition();
    sf::Vector2f bgSize = background.getSize();

    // Создаем FloatRect для SFML 3.0
    sf::FloatRect bgRect({ bgPos.x, bgPos.y }, { bgSize.x, bgSize.y });

    // Используем contains с sf::Vector2f
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