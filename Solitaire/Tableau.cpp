#include "Tableau.h"

Tableau::Tableau() : Tableau({ 0, 0 }) {}
Tableau::Tableau(sf::Vector2f pos) : Pile(pos, 25.0f) {}  // Маленький отступ

bool Tableau::canAddCard(const Card& card) const {
    if (cards.empty()) {
        // В пустую Tableau можно положить только короля
        return card.getRank() == Rank::King;
    }

    // В непустую Tableau:
    // 1. Карты должны быть разного цвета
    // 2. Новая карта должна быть на единицу МЕНЬШЕ верхней
    const Card& topCard = getTopCard();
    return (card.isRed() != topCard.isRed()) &&
        (static_cast<int>(card.getRank()) + 1 ==
            static_cast<int>(topCard.getRank()));
}

bool Tableau::canTakeCardFrom(int index) const {
    if (index < 0 || index >= cards.size()) {
        return false;
    }

    // Можно взять только если карта открыта
    // и все карты выше нее тоже открыты
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

    // Копируем карты
    std::vector<Card> takenCards(cards.begin() + startIndex, cards.end());

    // Удаляем из стопки
    cards.erase(cards.begin() + startIndex, cards.end());

    return takenCards;
}

//bool Tableau::canMoveGroup(int startIndex, const Pile& target) const {
//    if (!canTakeCardFrom(startIndex)) {
//        return false;
//    }
//
//    // Проверяем, можно ли положить первую карту группы в целевую стопку
//    const Card& firstCard = cards[startIndex];
//
//    // Если целевая стопка - тоже Tableau
//    if (auto tableauTarget = dynamic_cast<const Tableau*>(&target)) {
//        return tableauTarget->canAddCard(firstCard);
//    }
//    // Если целевая стопка - Foundation
//    else if (auto foundationTarget = dynamic_cast<const Foundation*>(&target)) {
//        // В Foundation можно класть только по одной карте
//        if (startIndex != cards.size() - 1) {
//            return false;  // Группу в Foundation нельзя
//        }
//        return foundationTarget->canAddCard(firstCard);
//    }
//
//    return false;
//}