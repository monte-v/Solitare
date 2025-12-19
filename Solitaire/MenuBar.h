#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

class MenuBar {
private:
    std::shared_ptr<sf::Font> font;

    sf::RectangleShape background;
    sf::RectangleShape hintButton;

    std::optional<sf::Text> newGameText;  
    std::optional<sf::Text> helpText;
    std::optional<sf::Text> hintText;

    std::function<void()> onNewGameCallback;
    std::function<void()> onHelpCallback;
    std::function<void()> onHintCallback;

public:
    MenuBar();

    bool loadFont(const std::string& fontPath = "");
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);

    void setOnHintCallback(const std::function<void()>& callback);

    void handleClick(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const { return background.getGlobalBounds(); }
};