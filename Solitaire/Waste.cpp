#include "Waste.h"

Waste::Waste(sf::Vector2f pos)
    : Pile(pos, 0.0f, 20.0f) { 
}

bool Waste::canAddCard(const Card& card) const {
    return false;
}

bool Waste::canTakeCardFrom(int index) const {
    return index == static_cast<int>(cards.size()) - 1;
}

void Waste::draw(sf::RenderTarget& target) const {
    if (cards.empty() && emptyPileTexture) {
        const_cast<Waste*>(this)->initEmptyPileSprite();
        if (emptyPileSprite) {
            target.draw(*emptyPileSprite);
        }
    }

    for (size_t i = 0; i < cards.size(); ++i) {
        sf::Vector2f pos = position;
        pos.x += i * horizontalSpacing;

        Card& mutableCard = const_cast<Card&>(cards[i]);
        mutableCard.setPosition(pos);
        cards[i].draw(target);
    }
}