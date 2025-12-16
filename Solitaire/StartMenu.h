//#pragma once
//#include <SFML/Graphics.hpp>
//#include <functional>
//
//class StartMenu {
//private:
//    bool visible;
//    sf::Font font;
//
//    struct Button {
//        sf::RectangleShape shape;
//        std::optional<sf::Text> text;
//        std::function<void()> onClick;
//
//        Button() = default;
//
//        Button(const std::string& label, const sf::Vector2f& position,
//            const sf::Vector2f& size, const sf::Font& font) {
//            shape.setSize(size);
//            shape.setPosition(position);
//            shape.setFillColor(sf::Color(70, 130, 180));
//            shape.setOutlineColor(sf::Color::White);
//            shape.setOutlineThickness(2.0f);
//
//            text->setFont(font);
//            text->setString(label);
//            text->setCharacterSize(24);
//            text->setFillColor(sf::Color::White);
//
//            // Центрируем текст
//            //sf::FloatRect bounds = text->getLocalBounds();
//            //text->setOrigin({ bounds.width / 2.0f, bounds.height / 2.0f });
//
//            //sf::Vector2f boundsSize = text->getLocalBounds().getSize();
//            //text->setOrigin({ boundsSize.x / 2.0f, boundsSize.y / 2.0f });
//
//            //float textWidth = text->getLocalBounds().width;  // Получаем ширину
//            //float textHeight = text.getLocalBounds().height; // Получаем высоту
//
//
//            text->setPosition({
//                position.x + size.x / 2.0f,
//                position.y + size.y / 2.0f - 5.0f
//                });
//        }
//
//        bool contains(const sf::Vector2f& point) const {
//            return shape.getGlobalBounds().contains(point);
//        }
//
//        void draw(sf::RenderWindow& window) const {
//            window.draw(shape);
//            window.draw(*text);
//        }
//    };
//
//    Button startButton;
//    Button helpButton;
//    Button exitButton;
//    std::optional<sf::Text> title;
//
//public:
//    StartMenu();
//
//    bool loadFont(const std::string& path = "assets/fonts/arial.ttf");
//
//    void setOnStart(const std::function<void()>& callback) { startButton.onClick = callback; }
//    void setOnHelp(const std::function<void()>& callback) { helpButton.onClick = callback; }
//    void setOnExit(const std::function<void()>& callback) { exitButton.onClick = callback; }
//
//    void show() { visible = true; }
//    void hide() { visible = false; }
//    bool isVisible() const { return visible; }
//
//    void handleClick(const sf::Vector2f& mousePos);
//    void draw(sf::RenderWindow& window) const;
//};


#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Button.h"

class StartMenu {
private:
    bool visible;
    sf::Font font;

    Button startButton;
    Button helpButton;
    Button exitButton;

    std::optional<sf::Text> title;

public:
    StartMenu();

    bool initialize(const std::string& fontPath = "assets/fonts/arial.ttf");

    void setOnStart(const std::function<void()>& callback) {
        startButton.setOnClick(callback);
    }

    void setOnHelp(const std::function<void()>& callback) {
        helpButton.setOnClick(callback);
    }

    void setOnExit(const std::function<void()>& callback) {
        exitButton.setOnClick(callback);
    }

    void show() { visible = true; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void handleClick(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window) const;

private:
    void createUI();
};