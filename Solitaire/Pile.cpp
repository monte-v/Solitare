#include "Pile.h"
#include <algorithm>
std::shared_ptr<sf::Texture> Pile::emptyPileTexture = nullptr;

Pile::Pile(sf::Vector2f pos, float vSpacing, float hSpacing) : position(pos), verticalSpacing(vSpacing),
horizontalSpacing(hSpacing), emptyPileSprite(nullptr) {}

Pile::Pile(const Pile& other)
    : cards(other.cards), 
    position(other.position),
    verticalSpacing(other.verticalSpacing),
    horizontalSpacing(other.horizontalSpacing),
    emptyPileSprite(nullptr)  { }

Pile& Pile::operator=(const Pile& other) {
    if (this != &other) {
        cards = other.cards;
        position = other.position;
        verticalSpacing = other.verticalSpacing;
        horizontalSpacing = other.horizontalSpacing;

        emptyPileSprite.reset();
    }
    return *this;
}

Pile::Pile(Pile&& other) noexcept
    : cards(std::move(other.cards)),
    position(std::move(other.position)),
    verticalSpacing(std::move(other.verticalSpacing)),
    horizontalSpacing(std::move(other.horizontalSpacing)),
    emptyPileSprite(std::move(other.emptyPileSprite)) { }

Pile& Pile::operator=(Pile&& other) noexcept {
    if (this != &other) {
        cards = std::move(other.cards);
        position = std::move(other.position);
        verticalSpacing = std::move(other.verticalSpacing);
        horizontalSpacing = std::move(other.horizontalSpacing);
        emptyPileSprite = std::move(other.emptyPileSprite);
    }
    return *this;
}

bool Pile::loadEmptyPileTexture(const std::string& path)
{
    if (emptyPileTexture) {
        return true; 
    }

    emptyPileTexture = std::make_shared<sf::Texture>();
    if (emptyPileTexture->loadFromFile(path)) {
        std::cout << "Текстура пустой стопки загружена: " << path << std::endl;
        return true;
    }
    else {
        std::cerr << "Ошибка загрузки текстуры пустой стопки: " << path << std::endl;
        emptyPileTexture.reset();
        return false;
    }
}

void Pile::initEmptyPileSprite() {
    if (!emptyPileSprite && emptyPileTexture) {
        emptyPileSprite = std::make_unique<sf::Sprite>(*emptyPileTexture);

        sf::Vector2u texSize = emptyPileTexture->getSize();
        if (texSize.x > 0 && texSize.y > 0) {
            emptyPileSprite->setScale(sf::Vector2f(
                static_cast<float>(Card::WIDTH) / texSize.x,
                static_cast<float>(Card::HEIGHT) / texSize.y
            ));
        }

        emptyPileSprite->setPosition(position);
    }
}

void Pile::output() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". ";
        cards[i].output(); 
    }
}

void Pile::addCard(Card card) {
    card.setPosition(getCardPosition(cards.size()));
    cards.push_back(card);

}

Card Pile::removeTopCard() {
    if (cards.empty()) {
        throw std::runtime_error("Стопка пуста!");
    }

    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}

Card Pile::takeCardAt(int index) {
    if (index < 0 || index >= cards.size()) {
        throw std::runtime_error("Неверный индекс карты!");
    }

    Card card = cards[index];
    cards.erase(cards.begin() + index, cards.end());
    return card;
}

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

Card& Pile::getCardAt(int index) {
    if (index < 0 || index >= cards.size()) {
        throw std::runtime_error("Неверный индекс карты!");
    }
    return cards[index];
}

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

    for (int i = cards.size() - 1; i >= 0; i--) {
        if (cards[i].contains(point)) {
            return i;
        }
    }
    return -1;  
}

// Проверка попадания в границы стопки
bool Pile::contains(sf::Vector2f point) const {
    /*if (cards.empty()) {
        if (emptyPileTexture) {
            const_cast<Pile*>(this)->initEmptyPileSprite();
            if (emptyPileSprite) {
                return emptyPileSprite->getGlobalBounds().contains(point);
            }
        }

        sf::FloatRect bounds(position,
            sf::Vector2f(Card::WIDTH, Card::HEIGHT));
        return bounds.contains(point);
    }
    return cards.back().contains(point);*/

    if (cards.empty()) {
        // Для пустой стопки
        if (emptyPileTexture) {
            const_cast<Pile*>(this)->initEmptyPileSprite();
            if (emptyPileSprite) {
                return emptyPileSprite->getGlobalBounds().contains(point);
            }
        }

        sf::FloatRect bounds(position,
            sf::Vector2f(Card::WIDTH, Card::HEIGHT));
        return bounds.contains(point);
    }

    // ПРОВЕРЯЕМ ВСЕ КАРТЫ СВЕРХУ ВНИЗ
    for (int i = cards.size() - 1; i >= 0; i--) {
        if (cards[i].contains(point)) {
            return true;
        }
    }

    return false;
}

// Открыть верхнюю карту
void Pile::revealTopCard() {
    if (!cards.empty() && !cards.back().isFaceUp()) {
        std::cout << "Стопка не пуста и последняя карта рубашкой вверх" << std::endl;
        cards.back().setFaceUp(true);
    }
}

void Pile::draw(sf::RenderTarget& target) const {
    if (cards.empty() && emptyPileTexture) {
        const_cast<Pile*>(this)->initEmptyPileSprite();

        if (emptyPileSprite) {
            target.draw(*emptyPileSprite);
        }
    }

    for (const auto& card : cards) {
        card.draw(target);
    }
}

std::vector<Card> Pile::takeCardsFrom(int startIndex) {
    if (startIndex < 0 || startIndex >= cards.size()) {
        throw std::runtime_error("Неверный индекс!");
    }

    std::vector<Card> takenCards;
    for (int i = startIndex; i < cards.size(); i++) {
        takenCards.push_back(cards[i]);
    }

    cards.erase(cards.begin() + startIndex, cards.end());
    return takenCards;
}