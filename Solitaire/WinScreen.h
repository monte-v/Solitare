#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WinScreen {
private:
    sf::RectangleShape background;
    sf::RectangleShape panel;

    sf::Font* font;
    std::optional<sf::Text> winText;
    std::optional<sf::Text> timeText;
    std::optional<sf::Text> clickText;

    bool visible;

public:
    WinScreen();
    ~WinScreen();

    bool loadFont(const std::string& fontPath);
    void show(const std::string& time);
    void hide();
    bool isVisible() const { return visible; }

    void setPosition(float x, float y);
    void setSize(float width, float height);

    bool handleClick(float mouseX, float mouseY);
    void draw(sf::RenderWindow& window) const;
};
