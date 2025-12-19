#pragma once
#include "Pile.h"
#include "Enums.h"

class Foundation : public Pile {
private:
    Suit suit;
    static std::array<std::shared_ptr<sf::Texture>, 4> suitTextures;
public:
    Foundation();
    Foundation(Suit s, sf::Vector2f pos);

    Foundation(const Foundation& other);
    Foundation& operator=(const Foundation& other);

    Foundation(Foundation&& other) noexcept;
    Foundation& operator=(Foundation&& other) noexcept;

    bool canAddCard(const Card& card) const override;
    bool canTakeCardFrom(int index) const override { return false; }

    Suit getSuit() const { return suit; }
    bool isComplete() const;

    static bool loadTextures(const std::string& basePath);
    void draw(sf::RenderTarget& target) const override;
};