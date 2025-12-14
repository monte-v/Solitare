#pragma once
#include "Pile.h"
#include "Enums.h"

class Foundation : public Pile {
private:
    Suit suit;

public:
    Foundation();
    Foundation(Suit s, sf::Vector2f pos);

    // Правила Foundation
    bool canAddCard(const Card& card) const override;
    bool canTakeCardFrom(int index) const override { return false; }
    //bool canTakeCard() const override;  // Можно брать только если не нарушится последовательность

    // Дополнительные методы
    Suit getSuit() const { return suit; }
    bool isComplete() const;

private:
    //void updateLayout() override;  // Карты лежат вплотную
};