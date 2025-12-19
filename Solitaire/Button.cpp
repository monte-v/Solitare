#include "Button.h"

Button::Button(const std::string& label, const sf::Vector2f& position,
    const sf::Vector2f& size, const sf::Font& font) {
    setup(position, size);
    setText(label, font);
}

void Button::setup(const sf::Vector2f& position, const sf::Vector2f& size) {
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(70, 130, 180));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.0f);
}

void Button::setText(const std::string& label, const sf::Font& font) {
    if (!text.has_value()) {
        text.emplace(font);
    }

    text->setString(label);
    text->setCharacterSize(24);
    text->setFillColor(sf::Color::White);

    sf::FloatRect textBounds = text->getLocalBounds();
    sf::FloatRect buttonBounds = shape.getGlobalBounds();

    float posX = buttonBounds.position.x + (buttonBounds.size.x - textBounds.size.x) / 2.0f;
    float posY = buttonBounds.position.y + (buttonBounds.size.y - textBounds.size.y) / 2.0f;

    posX -= textBounds.position.x;
    posY -= textBounds.position.y;

    text->setPosition({ posX, posY });
}

void Button::setOnClick(const std::function<void()>& callback) {
    onClick = callback;
}

bool Button::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

void Button::click() {
    if (onClick) {
        onClick();
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    if (text.has_value()) {
        window.draw(*text);
    }
}

void Button::setColor(const sf::Color& color) {
    shape.setFillColor(color);
}