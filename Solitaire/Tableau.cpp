#include "Tableau.h"

Tableau::Tableau() : Tableau({ 0, 0 }) {}
Tableau::Tableau(sf::Vector2f pos) : Pile(pos, 25.0f) {}  

Tableau::Tableau(const Tableau& other) : Pile(other) {}

Tableau& Tableau::operator=(const Tableau& other) {
    Pile::operator=(other);
    return *this;
}

Tableau::Tableau(Tableau&& other) noexcept : Pile(std::move(other)) {}

Tableau& Tableau::operator=(Tableau&& other) noexcept {
    Pile::operator=(std::move(other));
    return *this;
}

bool Tableau::canAddCard(const Card& card) const {
    if (cards.empty()) {
        return card.getRank() == Rank::King;
    }

    const Card& topCard = getTopCard();
    return (card.isRed() != topCard.isRed()) &&
        (static_cast<int>(card.getRank()) + 1 ==
            static_cast<int>(topCard.getRank()));
}

bool Tableau::canTakeCardFrom(int index) const {
    if (index < 0 || index >= cards.size()) {
        return false;
    }

    for (int i = index; i < cards.size(); i++) {      
        if (!cards[i].isFaceUp()) {
            return false;
        }
    }

    return true;
}

std::vector<Card> Tableau::takeCardsFrom(int startIndex) {
    if (!canTakeCardFrom(startIndex)) {
        throw std::runtime_error("Нельзя взять карты с этого индекса!");
    }

    std::vector<Card> takenCards(cards.begin() + startIndex, cards.end());

    cards.erase(cards.begin() + startIndex, cards.end());

    return takenCards;
}

