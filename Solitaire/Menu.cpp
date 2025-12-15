#include "Menu.h"
#include <iostream>

Menu::Menu()
    : selectedIndex(0),
    alignment(Alignment::Center),
    position(0, 0),
    spacing(50.0f),
    visible(false),
    animationTime(0),
    isAnimating(false) {

    // Настройки по умолчанию
    background.setFillColor(sf::Color(0, 0, 0, 200));
}

bool Menu::loadFont(const std::string& fontPath) {
    return font.loadFromFile(fontPath);
}

void Menu::setFont(const sf::Font& newFont) {
    font = newFont;
    for (auto& item : items) {
        item.sfText.setFont(font);
    }
    updateItemsPosition();
}

void Menu::setPosition(const sf::Vector2f& pos) {
    position = pos;
    updateItemsPosition();
}

void Menu::setSpacing(float space) {
    spacing = space;
    updateItemsPosition();
}

void Menu::setAlignment(Alignment align) {
    alignment = align;
    updateItemsPosition();
}

void Menu::setBackground(const sf::Color& color, float opacity) {
    sf::Color bgColor = color;
    bgColor.a = static_cast<sf::Uint8>(opacity * 255);
    background.setFillColor(bgColor);
}

void Menu::addItem(const std::string& text, std::function<void()> action, bool enabled) {
    addItem(text, action, sf::Color::White, sf::Color::Yellow, enabled);
}

void Menu::addItem(const std::string& text, std::function<void()> action,
    const sf::Color& normalColor, const sf::Color& selectedColor,
    bool enabled) {
    MenuItem item;
    item.text = text;
    item.action = action;
    item.normalColor = normalColor;
    item.selectedColor = selectedColor;
    item.enabled = enabled;

    item.sfText.setString(text);
    item.sfText.setFont(font);
    item.sfText.setCharacterSize(36);
    item.sfText.setFillColor(enabled ? normalColor : sf::Color(128, 128, 128));

    items.push_back(item);
    updateItemsPosition();
}

void Menu::removeItem(int index) {
    if (index >= 0 && index < items.size()) {
        items.erase(items.begin() + index);
        if (selectedIndex >= items.size()) {
            selectedIndex = items.size() - 1;
        }
        updateItemsPosition();
    }
}

void Menu::clear() {
    items.clear();
    selectedIndex = 0;
}

void Menu::show() {
    visible = true;
    isAnimating = true;
    animationTime = 0;
}

void Menu::hide() {
    visible = false;
}

void Menu::toggle() {
    visible = !visible;
    if (visible) {
        isAnimating = true;
        animationTime = 0;
    }
}

void Menu::update(sf::Time deltaTime) {
    if (isAnimating) {
        animationTime += deltaTime.asSeconds();
        if (animationTime > 0.3f) { // 0.3 секунды анимации
            isAnimating = false;
        }
    }
}

void Menu::draw(sf::RenderTarget& target) const {
    if (!visible) return;

    // Рисуем фон
    target.draw(background);

    // Рисуем все пункты меню
    for (const auto& item : items) {
        target.draw(item.sfText);
    }
}

void Menu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!visible) return;

    if (event.is<sf::Event::KeyPressed>()) {
        auto* keyEvent = event.getIf<sf::Event::KeyPressed>();

        switch (keyEvent->code) {
        case sf::Keyboard::Up:
            moveUp();
            break;
        case sf::Keyboard::Down:
            moveDown();
            break;
        case sf::Keyboard::Enter:
        case sf::Keyboard::Space:
            selectCurrent();
            break;
        case sf::Keyboard::Escape:
            hide();
            break;
        }
    }
    else if (auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Vector2i(mouseMove->position.x, mouseMove->position.y));

        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].enabled && items[i].sfText.getGlobalBounds().contains(mousePos)) {
                selectedIndex = i;
                updateItemsColors();
                break;
            }
        }
    }
    else if (auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(mouseClick->position.x, mouseClick->position.y));

            for (size_t i = 0; i < items.size(); i++) {
                if (items[i].enabled && items[i].sfText.getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                    selectCurrent();
                    break;
                }
            }
        }
    }
}

void Menu::moveUp() {
    do {
        selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    } while (!items[selectedIndex].enabled && selectedIndex != 0);

    updateItemsColors();
}

void Menu::moveDown() {
    do {
        selectedIndex = (selectedIndex + 1) % items.size();
    } while (!items[selectedIndex].enabled);

    updateItemsColors();
}

void Menu::selectCurrent() {
    if (selectedIndex >= 0 && selectedIndex < items.size() &&
        items[selectedIndex].enabled && items[selectedIndex].action) {
        items[selectedIndex].action();
    }
}

void Menu::updateItemsPosition() {
    // Обновляем размер фона
    float totalHeight = items.size() * spacing;
    float maxWidth = 0;

    for (const auto& item : items) {
        sf::FloatRect bounds = item.sfText.getLocalBounds();
        if (bounds.width > maxWidth) {
            maxWidth = bounds.width;
        }
    }

    background.setSize(sf::Vector2f(maxWidth + 100, totalHeight + 50));
    background.setPosition(position.x - (maxWidth + 100) / 2,
        position.y - 25);

    // Позиционируем текст
    for (size_t i = 0; i < items.size(); i++) {
        sf::Text& text = items[i].sfText;
        sf::FloatRect bounds = text.getLocalBounds();

        float x = position.x;
        float y = position.y + i * spacing - totalHeight / 2;

        // Выравнивание
        switch (alignment) {
        case Alignment::Center:
            text.setOrigin(bounds.width / 2, bounds.height / 2);
            break;
        case Alignment::Left:
            text.setOrigin(0, bounds.height / 2);
            x -= maxWidth / 2;
            break;
        case Alignment::Right:
            text.setOrigin(bounds.width, bounds.height / 2);
            x += maxWidth / 2;
            break;
        }

        text.setPosition(x, y);
    }

    updateItemsColors();
}

void Menu::updateItemsColors() {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].enabled) {
            items[i].sfText.setFillColor(
                i == selectedIndex ? items[i].selectedColor : items[i].normalColor
            );
        }
        else {
            items[i].sfText.setFillColor(sf::Color(128, 128, 128));
        }
    }
}