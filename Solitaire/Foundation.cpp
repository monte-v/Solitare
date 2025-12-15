#include "Foundation.h"

Foundation::Foundation() : Foundation(Suit::Hearts, { 0, 0 }) {}
Foundation::Foundation(Suit s, sf::Vector2f pos)
    : Pile(pos, 0.0f), suit(s) {
} 

Foundation::Foundation(const Foundation& other)
    : Pile(other), suit(other.suit) {
}

Foundation& Foundation::operator=(const Foundation& other) {
    if (this != &other) {
        Pile::operator=(other);
        suit = other.suit;
    }
    return *this;
}

Foundation::Foundation(Foundation&& other) noexcept
    : Pile(std::move(other)), suit(std::move(other.suit)) {
}

Foundation& Foundation::operator=(Foundation&& other) noexcept {
    if (this != &other) {
        Pile::operator=(std::move(other));
        suit = std::move(other.suit);
    }
    return *this;
}

bool Foundation::canAddCard(const Card& card) const {
    if (card.getSuit() != suit) {
        return false;
    }

    if (cards.empty()) {
        return card.getRank() == Rank::Ace;
    }

    const Card& topCard = getTopCard();
    return static_cast<int>(card.getRank()) ==
        static_cast<int>(topCard.getRank()) + 1;
}

bool Foundation::isComplete() const {
    return cards.size() == 13;
}