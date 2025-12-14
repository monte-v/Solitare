#include "Foundation.h"

Foundation::Foundation() : Foundation(Suit::Hearts, { 0, 0 }) {}
Foundation::Foundation(Suit s, sf::Vector2f pos)
    : Pile(pos, 0.0f), suit(s) {
}  // Карты лежат вплотную


// Конструктор копирования
Foundation::Foundation(const Foundation& other)
    : Pile(other), suit(other.suit) {
}

// Оператор присваивания копированием
Foundation& Foundation::operator=(const Foundation& other) {
    if (this != &other) {
        Pile::operator=(other);
        suit = other.suit;
    }
    return *this;
}

// Конструктор перемещения
Foundation::Foundation(Foundation&& other) noexcept
    : Pile(std::move(other)), suit(std::move(other.suit)) {
}

// Оператор присваивания перемещением
Foundation& Foundation::operator=(Foundation&& other) noexcept {
    if (this != &other) {
        Pile::operator=(std::move(other));
        suit = std::move(other.suit);
    }
    return *this;
}


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