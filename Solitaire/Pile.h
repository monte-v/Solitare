#pragma once
#include <vector>
#include <memory>
#include "Card.h"

class Pile {
protected:
    std::vector<Card> cards;
    sf::Vector2f position;
    float verticalSpacing;  // Расстояние между картами по вертикали
    float horizontalSpacing; // Для waste режима (по горизонтали)

public:
    Pile(sf::Vector2f pos, float vSpacing = 25.0f, float hSpacing = 10.0f);
    virtual ~Pile() = default;

    void output() const;

    // Виртуальные методы
    virtual bool canAddCard(const Card& card) const = 0;
    virtual bool canTakeCard() const { return !cards.empty(); }
    virtual bool canTakeCardFrom(int index) const { return false; }

    // Базовые методы
    void addCard(Card card);
    Card removeTopCard();
    Card takeCardAt(int index);
    std::vector<Card> takeCardsFrom(int startIndex);

    //// Геттеры
    std::vector<Card>& getCardsRef() { return cards; }
    Card& getTopCard();
    const Card& getTopCard() const;
    Card& getCardAt(int index);
    const std::vector<Card>& getCards() const { return cards; }
    bool isEmpty() const { return cards.empty(); }
    size_t getCardCount() const { return cards.size(); }

    // Графика
    //void setPosition(sf::Vector2f pos) { position = pos; updateCardPositions(); }
    //sf::Vector2f getPosition() const { return position; }

    //// Позиционирование карт
    sf::Vector2f getCardPosition(int index) const;
    //void updateCardPositions();

    //// Поиск и взаимодействие
    int getCardIndexAt(sf::Vector2f point) const;
    bool contains(sf::Vector2f point) const;

    //// Утилиты
    void revealTopCard();
    int getFaceUpCount() const;

    // Отрисовка (SFML 3)
    virtual void draw(sf::RenderTarget& target) const;

protected:
    //virtual void updateLayout();  // Для кастомизации расположения
};