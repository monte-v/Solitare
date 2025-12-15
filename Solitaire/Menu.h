#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>

class Menu {
public:
    struct MenuItem {
        std::string text;
        std::function<void()> action;
        sf::Text sfText;
        sf::Color normalColor;
        sf::Color selectedColor;
        bool enabled;
    };

    enum class Alignment {
        Center,
        Left,
        Right
    };

private:
    std::vector<MenuItem> items;
    int selectedIndex;
    sf::Font font;
    Alignment alignment;
    sf::Vector2f position;
    float spacing;
    bool visible;

    // Для анимации
    float animationTime;
    bool isAnimating;
    sf::RectangleShape background;

public:
    Menu();

    bool loadFont(const std::string& fontPath);
    void setFont(const sf::Font& newFont);
    void setPosition(const sf::Vector2f& pos);
    void setSpacing(float space);
    void setAlignment(Alignment align);
    void setBackground(const sf::Color& color, float opacity = 0.8f);

    void addItem(const std::string& text, std::function<void()> action,
        bool enabled = true);
    void addItem(const std::string& text, std::function<void()> action,
        const sf::Color& normalColor, const sf::Color& selectedColor,
        bool enabled = true);

    void removeItem(int index);
    void clear();

    void show();
    void hide();
    void toggle();
    bool isVisible() const { return visible; }

    void update(sf::Time deltaTime);
    void draw(sf::RenderTarget& target) const;

    // Обработка ввода
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    void selectCurrent();

    // Геттеры
    int getSelectedIndex() const { return selectedIndex; }
    size_t getItemCount() const { return items.size(); }
    const MenuItem& getItem(int index) const { return items[index]; }

private:
    void updateItemsPosition();
    void updateItemsColors();
};