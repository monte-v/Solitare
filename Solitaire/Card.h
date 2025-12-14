#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Enums.h"
#include <iostream>

class Card {
public:
    static constexpr int WIDTH = 100;
    static constexpr int HEIGHT = 140;

private:
    Suit suit;
    Rank rank;
    bool faceUp;

    static std::shared_ptr<sf::Texture> frontTexture;
    static std::shared_ptr<sf::Texture> backTexture;
    sf::Sprite* sprite;

public:
    Card(Suit s, Rank r);
    ~Card();
    Card(const Card&) noexcept;
    Card& operator=(const Card&) noexcept;

    // Разрешаем перемещение (если нужно)
    Card(Card&& other) noexcept;
    Card& operator=(Card&& other) noexcept;

    void output() const;

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    bool isFaceUp() const { return faceUp; }

    //// Логика
    bool isRed() const;
    bool canPlaceOn(const Card& other) const;

    // Графика
    static bool loadTextures(const std::string& frontPath, const std::string& backPath);
    void flip();
    void setFaceUp(bool up);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    //// SFML 3: draw теперь принимает sf::RenderTarget&
    void draw(sf::RenderTarget& target) const;

    //// Проверка попадания (SFML 3: contains вместо getGlobalBounds().contains)
    bool contains(sf::Vector2f point) const;

    //// Для сериализации/отладки
    //std::string toString() const;
};


