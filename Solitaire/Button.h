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
    Button() = default;

    Button(const std::string& label, const sf::Vector2f& position,
        const sf::Vector2f& size, const sf::Font& font);

    void setup(const sf::Vector2f& position, const sf::Vector2f& size);
    void setText(const std::string& label, const sf::Font& font);

    void setOnClick(const std::function<void()>& callback);
    bool contains(const sf::Vector2f& point) const;
    void click();

    void draw(sf::RenderWindow& window) const;
    void setColor(const sf::Color& color);
};


