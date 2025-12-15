#include "Stock.h"

Stock::Stock(sf::Vector2f pos) : Pile(pos, 0.0f), wasteMode(false) {}

Card Stock::drawCard() {
    if (cards.empty()) {
        throw std::runtime_error("Сток пуст!");
    }

    Card card = removeTopCard();
    //card.setFaceUp(true); 
    return card;
}

void Stock::reset(const std::vector<Card>& newCards) {
    cards.clear();
    for (const auto& card : newCards) {
        addCard(card);
    }
}

std::vector<Card> Stock::getVisibleCards() const {
    if (!wasteMode || cards.empty()) {
        return cards;
    }

    int startIndex = std::max(0, (int)cards.size() - 3);
    std::vector<Card> visible;
    for (int i = startIndex; i < cards.size(); i++) {
        visible.push_back(cards[i]);
    }
    return visible;
}