#pragma once
#include "Pile.h"

class Tableau : public Pile {
public:
    Tableau();
    Tableau(sf::Vector2f pos);

    Tableau(const Tableau& other);
    Tableau& operator=(const Tableau& other);

    Tableau(Tableau&& other) noexcept;
    Tableau& operator=(Tableau&& other) noexcept;

    bool canAddCard(const Card& card) const override;
    bool canTakeCard() const override { return !cards.empty(); }
    bool canTakeCardFrom(int index) const override;

    std::vector<Card> takeCardsFrom(int startIndex);
};