#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

class MenuBar {
private:
    sf::RectangleShape background;

    sf::RectangleShape newGameButton;
    sf::RectangleShape helpButton;
    sf::RectangleShape hintButton;

    std::optional<sf::Text> newGameText;  
    std::optional<sf::Text> helpText;
    std::optional<sf::Text> hintText;

    std::shared_ptr<sf::Font> font;

    std::function<void()> onNewGameCallback;
    std::function<void()> onHelpCallback;
    std::function<void()> onHintCallback;

public:
    MenuBar();

    bool loadFont(const std::string& fontPath = "");
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);

    void setOnNewGameCallback(const std::function<void()>& callback);
    void setOnHelpCallback(const std::function<void()>& callback);
    void setOnHintCallback(const std::function<void()>& callback);

    void handleClick(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const { return background.getGlobalBounds(); }
};