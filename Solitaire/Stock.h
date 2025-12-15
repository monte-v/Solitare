#pragma once
#include "Pile.h"

class Stock : public Pile {
private:
    bool wasteMode;  // Режим "отходов" (в косынке обычно 3 карты)

public:
    Stock(sf::Vector2f pos);

    bool canAddCard(const Card& card) const override { return true; }
    bool canTakeCard() const override { return !cards.empty(); }

    Card drawCard();
    void reset(const std::vector<Card>& newCards);

    void setWasteMode(bool waste) { wasteMode = waste; }
    bool isWasteMode() const { return wasteMode; }

    std::vector<Card> getVisibleCards() const;
};