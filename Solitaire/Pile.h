#pragma once
#include <vector>
#include <memory>
#include "Card.h"

class Pile {
protected:
    std::vector<Card> cards;
    sf::Vector2f position;
    float verticalSpacing;  
    float horizontalSpacing; 
    
    static std::shared_ptr<sf::Texture> emptyPileTexture;
    std::unique_ptr<sf::Sprite> emptyPileSprite;

public:
    Pile(sf::Vector2f pos, float vSpacing = 25.0f, float hSpacing = 10.0f);
    Pile(Pile&& other) noexcept;  
    Pile& operator=(Pile&& other) noexcept;  
    Pile(const Pile& other);  
    Pile& operator=(const Pile& other);  
    virtual ~Pile() = default;

    static bool loadEmptyPileTexture(const std::string& path);

    void output() const;

    virtual bool canAddCard(const Card& card) const = 0;
    virtual bool canTakeCard() const { return !cards.empty(); }
    virtual bool canTakeCardFrom(int index) const { return false; }

    void addCard(Card card);
    Card removeTopCard();
    Card takeCardAt(int index);
    std::vector<Card> takeCardsFrom(int startIndex);

    std::vector<Card>& getCardsRef() { return cards; }
    Card& getTopCard();
    const Card& getTopCard() const;
    Card& getCardAt(int index);
    const std::vector<Card>& getCards() const { return cards; }
    bool isEmpty() const { return cards.empty(); }
    size_t getCardCount() const { return cards.size(); }
    sf::Vector2f getCardPosition(int index) const;
    int getCardIndexAt(sf::Vector2f point) const;
    int getFaceUpCount() const;

    bool contains(sf::Vector2f point) const;

    void revealTopCard();

    virtual void draw(sf::RenderTarget& target) const;

protected:
    void initEmptyPileSprite();
};