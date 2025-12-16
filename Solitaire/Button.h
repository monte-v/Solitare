#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    std::optional<sf::Text> text;
    std::function<void()> onClick;

public:
    // Конструктор по умолчанию
    Button() = default;

    // Конструктор для быстрого создания
    Button(const std::string& label, const sf::Vector2f& position,
        const sf::Vector2f& size, const sf::Font& font) {
        setup(position, size);
        setText(label, font);
    }

    // Основной метод настройки
    void setup(const sf::Vector2f& position, const sf::Vector2f& size) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(70, 130, 180));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);
    }

    // Установка текста
    void setText(const std::string& label, const sf::Font& font) {
        // Инициализируем optional если он пустой
        if (!text.has_value()) {
            text.emplace(font);
        }
        text->setString(label);
        text->setCharacterSize(24);
        text->setFillColor(sf::Color::White);

        // Простое позиционирование
        text->setPosition({
            shape.getPosition().x + 20.0f,
            shape.getPosition().y + 15.0f
            });
    }

    // Установка callback
    void setOnClick(const std::function<void()>& callback) {
        onClick = callback;
    }

    // Проверка попадания мыши
    bool contains(const sf::Vector2f& point) const {
        return shape.getGlobalBounds().contains(point);
    }

    // Обработка клика
    void click() {
        if (onClick) {
            onClick();
        }
    }

    // Отрисовка
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
        if (text.has_value()) {
            window.draw(*text);
        }
    }

    // Опционально: изменение цвета
    void setColor(const sf::Color& color) {
        shape.setFillColor(color);
    }
};