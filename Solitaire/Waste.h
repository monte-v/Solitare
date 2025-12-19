#pragma once
#include "Pile.h"

class Waste : public Pile {
public:
    Waste(sf::Vector2f pos);

    bool canAddCard(const Card& card) const override;
    bool canTakeCard() const override { return !cards.empty(); }
    bool canTakeCardFrom(int index) const override;

    void draw(sf::RenderTarget& target) const override;
};