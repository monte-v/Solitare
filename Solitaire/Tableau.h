#pragma once
#include "Pile.h"

class Tableau : public Pile {
public:
    Tableau();
    Tableau(sf::Vector2f pos);

    // Правила Tableau
    bool canAddCard(const Card& card) const override;
    bool canTakeCard() const override { return !cards.empty(); }
    bool canTakeCardFrom(int index) const override;

    std::vector<Card> takeCardsFrom(int startIndex);

    // Особенности Tableau
    //bool canMoveGroup(int startIndex, const Pile& target) const;

private:
    //void updateLayout() override;  // Каскадное расположение карт
};