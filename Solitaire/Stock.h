#pragma once
#include "Pile.h"

class Stock : public Pile {
private:
    bool wasteMode;  

public:
    Stock(sf::Vector2f pos);

    bool canAddCard(const Card& card) const override { return false; }
    bool canTakeCard() const override { return false; }

    Card drawCard();
    void reset(const std::vector<Card>& newCards);

    void setWasteMode(bool waste) { wasteMode = waste; }
    bool isWasteMode() const { return wasteMode; }

    std::vector<Card> getVisibleCards() const;
};