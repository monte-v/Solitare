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

    static std::shared_ptr<sf::Texture> backTexture;
    static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> frontTextureCache;
    sf::Sprite* sprite;

    std::string getTextureFileName() const;
    std::string getTextureKey() const;
    bool loadFrontTexture();

public:
    Card(Suit s, Rank r);
    ~Card();
    Card(const Card&) noexcept;
    Card& operator=(const Card&) noexcept;
    Card(Card&& other) noexcept;
    Card& operator=(Card&& other) noexcept;

    static bool loadBackTexture(const std::string& backPath);

    static void clearTextureCache();

    void output() const;

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    bool isFaceUp() const { return faceUp; }

    bool isRed() const;
    bool canPlaceOn(const Card& other) const;

    void flip();
    void setFaceUp(bool up);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderTarget& target) const;

    bool contains(sf::Vector2f point) const;
};


