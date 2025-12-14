#include "Pile.h"
#include <algorithm>
std::shared_ptr<sf::Texture> Pile::emptyPileTexture = nullptr;

Pile::Pile(sf::Vector2f pos, float vSpacing, float hSpacing) : position(pos), verticalSpacing(vSpacing),
horizontalSpacing(hSpacing), emptyPileSprite(nullptr) {
}

Pile::Pile(Pile&& other) noexcept
    : cards(std::move(other.cards)),
    position(std::move(other.position)),
    verticalSpacing(std::move(other.verticalSpacing)),
    horizontalSpacing(std::move(other.horizontalSpacing)),
    emptyPileSprite(std::move(other.emptyPileSprite))
{
    // other теперь в валидном, но пустом состоянии
}

// Оператор присваивания перемещением
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

// Конструктор копирования (глубокое копирование)
Pile::Pile(const Pile& other)
    : cards(other.cards),  // Card должен иметь корректный конструктор копирования
    position(other.position),
    verticalSpacing(other.verticalSpacing),
    horizontalSpacing(other.horizontalSpacing),
    emptyPileSprite(nullptr)  // Спрайт не копируем, создадим при необходимости
{
    // Карты копируются, спрайт будет создан лениво
}

// Оператор присваивания копированием
Pile& Pile::operator=(const Pile& other) {
    if (this != &other) {
        cards = other.cards;
        position = other.position;
        verticalSpacing = other.verticalSpacing;
        horizontalSpacing = other.horizontalSpacing;

        // Сбрасываем спрайт - будет создан при необходимости
        emptyPileSprite.reset();
    }
    return *this;
}

bool Pile::loadEmptyPileTexture(const std::string& path)
{
    if (emptyPileTexture) {
        return true; // Уже загружена
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
        // Создаем спрайт только при необходимости
        emptyPileSprite = std::make_unique<sf::Sprite>(*emptyPileTexture);

        // Масштабируем под размер карты
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
    //if (cards.empty()) {
    //    // Для пустой стопки проверяем попадание в область
    //    sf::FloatRect bounds(sf::Vector2f(position.x, position.y),
    //        sf::Vector2f(Card::WIDTH, Card::HEIGHT));
    //    return bounds.contains(point);
    //}

    //// Для непустой - проверяем попадание в верхнюю карту
    //return cards.back().contains(point);
    if (cards.empty()) {
        // Для пустой стопки
        if (emptyPileTexture) {
            const_cast<Pile*>(this)->initEmptyPileSprite();
            if (emptyPileSprite) {
                return emptyPileSprite->getGlobalBounds().contains(point);
            }
        }

        // Если нет текстуры, проверяем стандартную область
        sf::FloatRect bounds(position,
            sf::Vector2f(Card::WIDTH, Card::HEIGHT));
        return bounds.contains(point);
    }

    // Для непустой - проверяем попадание в верхнюю карту
    return cards.back().contains(point);

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
        // Ленивая инициализация спрайта
        const_cast<Pile*>(this)->initEmptyPileSprite();

        if (emptyPileSprite) {
            target.draw(*emptyPileSprite);
        }
    }

    for (const auto& card : cards) {
        // Предполагая, что Card имеет метод draw
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