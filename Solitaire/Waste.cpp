#include "Waste.h"

Waste::Waste(sf::Vector2f pos)
    : Pile(pos, 0.0f, 15.0f) { // verticalSpacing = 0, horizontalSpacing = 15
}

bool Waste::canAddCard(const Card& card) const {
    // В Waste можно добавлять только из Stock
    // Карты из других стопок нельзя класть в Waste
    // (это проверяется на уровне Game)
    return true;
}

bool Waste::canTakeCardFrom(int index) const {
    // Можно брать только верхнюю карту
    return index == static_cast<int>(cards.size()) - 1;
}

void Waste::draw(sf::RenderTarget& target) const {
    // Отрисовываем пустую стопку если нет карт
    if (cards.empty() && emptyPileTexture) {
        const_cast<Waste*>(this)->initEmptyPileSprite();
        if (emptyPileSprite) {
            target.draw(*emptyPileSprite);
        }
    }

    // Использовать const_cast для обхода ограничения const
    for (size_t i = 0; i < cards.size(); ++i) {
        sf::Vector2f pos = position;
        pos.x += i * horizontalSpacing;

        // Использовать const_cast для изменения позиции
        Card& mutableCard = const_cast<Card&>(cards[i]);
        mutableCard.setPosition(pos);
        cards[i].draw(target);
    }
}