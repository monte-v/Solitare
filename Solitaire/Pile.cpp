#include "Pile.h"
#include <algorithm>

Pile::Pile(sf::Vector2f pos, float vSpacing, float hSpacing) : position(pos), verticalSpacing(vSpacing),
horizontalSpacing(hSpacing) {
}

void Pile::output() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". ";
        cards[i].output(); // Предполагая, что Card имеет output()
    }
}

//Добавить карту в стопку
void Pile::addCard(Card card) {
    //cards.push_back(card);
    //card.setPosition(Pile::getCardPosition(cards.size() - 1));

    card.setPosition(getCardPosition(cards.size()));
    cards.push_back(card);

}


// Удалить верхнюю карту
Card Pile::removeTopCard() {
    if (cards.empty()) {
        throw std::runtime_error("Стопка пуста!");
    }

    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}

// Взять карту по индексу (и все карты выше нее)
// Особенно нужно для Tableau - можно взять не только верхнюю
Card Pile::takeCardAt(int index) {
    if (index < 0 || index >= cards.size()) {
        throw std::runtime_error("Неверный индекс карты!");
    }

    Card card = cards[index];
    cards.erase(cards.begin() + index, cards.end());
    return card;
}

// Получить верхнюю карту (без удаления)
Card& Pile::getTopCard() {
    if (cards.empty()) {
        throw std::runtime_error("Стопка пуста!");
    }
    return cards.back();
}

const Card& Pile::getTopCard() const {
    if (cards.empty()) {
        throw std::runtime_error("Стопка пуста!");
    }
    return cards.back();
}

// Получить карту по индексу
Card& Pile::getCardAt(int index) {
    if (index < 0 || index >= cards.size()) {
        throw std::runtime_error("Неверный индекс карты!");
    }
    return cards[index];
}

// Сколько карт открыто в стопке
int Pile::getFaceUpCount() const {
    int count = 0;
    for (const auto& card : cards) {
        if (card.isFaceUp()) count++;
    }
    return count;
}

// Позиция карты в стопке для отрисовки
sf::Vector2f Pile::getCardPosition(int cardIndex) const {
    float x = position.x;
    float y = position.y + (cardIndex * verticalSpacing);
    return sf::Vector2f(x, y);
}

// Найти индекс карты по координатам
int Pile::getCardIndexAt(sf::Vector2f point) const {
    if (cards.empty()) return -1;

    // Проверяем с верхней карты вниз
    for (int i = cards.size() - 1; i >= 0; i--) {
        if (cards[i].contains(point)) {
            return i;
        }
    }
    return -1;  // Не попали ни в одну карту
}

// Проверка попадания в границы стопки
bool Pile::contains(sf::Vector2f point) const {
    if (cards.empty()) {
        // Для пустой стопки проверяем попадание в область
        sf::FloatRect bounds(sf::Vector2f(position.x, position.y),
            sf::Vector2f(Card::WIDTH, Card::HEIGHT));
        return bounds.contains(point);
    }

    // Для непустой - проверяем попадание в верхнюю карту
    return cards.back().contains(point);
}

// Открыть верхнюю карту
void Pile::revealTopCard() {
    if (!cards.empty() && !cards.back().isFaceUp()) {
        cards.back().setFaceUp(true);
    }
}

void Pile::draw(sf::RenderTarget& target) const {
    for (const auto& card : cards) {
        // Предполагая, что Card имеет метод draw
        card.draw(target);
    }
}