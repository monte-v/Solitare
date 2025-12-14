#pragma once
#include "Pile.h"

class Stock : public Pile {
private:
    bool wasteMode;  // Режим "отходов" (в косынке обычно 3 карты)

public:
    Stock(sf::Vector2f pos);


    // В Stock можно положить любую карту 
    // (обычно только из Waste обратно)
    bool canAddCard(const Card& card) const override { return true; }
    bool canTakeCard() const override { return !cards.empty(); }

    // Особые методы для Stock
    Card drawCard();
    void reset(const std::vector<Card>& newCards);

    void setWasteMode(bool waste) { wasteMode = waste; }
    bool isWasteMode() const { return wasteMode; }

    // В wasteMode показываем только верхние 3 карты
    std::vector<Card> getVisibleCards() const;
};