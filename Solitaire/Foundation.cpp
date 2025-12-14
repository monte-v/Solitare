#include "Foundation.h"

Foundation::Foundation() : Foundation(Suit::Hearts, { 0, 0 }) {}
Foundation::Foundation(Suit s, sf::Vector2f pos)
    : Pile(pos, 0.0f), suit(s) {
}  // Карты лежат вплотную

bool Foundation::canAddCard(const Card& card) const {
    // 1. Проверяем масть
    if (card.getSuit() != suit) {
        return false;
    }

    if (cards.empty()) {
        // В пустую Foundation можно положить только туза
        return card.getRank() == Rank::Ace;
    }

    // В непустую Foundation:
    // Карта должна быть на единицу БОЛЬШЕ верхней
    const Card& topCard = getTopCard();
    return static_cast<int>(card.getRank()) ==
        static_cast<int>(topCard.getRank()) + 1;
}

bool Foundation::isComplete() const {
    // Foundation полная, если в ней 13 карт (от туза до короля)
    return cards.size() == 13;
}