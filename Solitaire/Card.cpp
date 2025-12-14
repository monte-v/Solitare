#include "Card.h"
#include <stdexcept>

Card::Card(Suit s, Rank r) : suit(s), rank(r), faceUp(false), sprite(nullptr) {}

void Card::output() const{
    std::array<std::string, 4> s_names = { "Hearts", "Diamonds", "Clubs", "Spades" };
    std::array<std::string, 14> r_names = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
        "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
    Rank r = rank;
    Suit s = suit;
    std::cout << r_names[static_cast<int>(r) - 1] << s_names[static_cast<int>(s)] << std::endl;
}

bool Card::isRed() const {
    return suit == Suit::Hearts || suit == Suit::Diamonds;
}

bool Card::canPlaceOn(const Card& other) const {
    return (isRed() != other.isRed()) &&
        (static_cast<int>(rank) + 1 == static_cast<int>(other.rank));
}

bool Card::loadTextures(const std::string& frontPath, const std::string& backPath) {
    frontTexture = std::make_shared<sf::Texture>();
    backTexture = std::make_shared<sf::Texture>();

    if (backTexture->loadFromFile(backPath) || frontTexture->loadFromFile(frontPath)) {
        std::cout << "Успешно загружен" << backPath << std::endl;
        // Успешно загрузили рубашку
        sprite = new sf::Sprite(faceUp ? *frontTexture : *backTexture);

        // Масштабируем спрайт под стандартные размеры карты
        sprite->setScale(sf::Vector2f(
            static_cast<float>(WIDTH) / frontTexture->getSize().x,
            static_cast<float>(HEIGHT) / frontTexture->getSize().y
        ));

        if (sprite) {
            std::cout << "Текстура карты загружена" << std::endl;
        }

        return true;
    }
    std::cerr << "Не удалось загрузить текстуру рубашки: " << backPath << std::endl;
    return false;
}

void Card::flip() {
    faceUp = !faceUp;
    //if (sprite) {
    //    if (faceUp && frontTexture) {
    //        sprite->setTexture(*frontTexture, true);
    //    }
    //    else if (!faceUp && backTexture) {
    //        sprite->setTexture(*backTexture, true);
    //    }
    //}
}

void Card::setFaceUp(bool up) {
    if (faceUp != up) {
        flip();
    }
}

void Card::setPosition(sf::Vector2f pos) {
    if (sprite) {
        sprite->setPosition(pos);
    }
}

sf::Vector2f Card::getPosition() const {
    if (sprite) {
        return sprite->getPosition();
    }
    return { 0, 0 };
}

void Card::draw(sf::RenderTarget& target) const {
    //if (sprite) {
    //    target.draw(*sprite);
    //}

    //std::cout << "Card::draw() - спрайт: " << (sprite ? "есть" : "nullptr")
    //    << ", позиция: " << (sprite ? sprite->getPosition().x : 0)
    //    << ", " << (sprite ? sprite->getPosition().y : 0) << std::endl;

    if (sprite) {
        target.draw(*sprite);
    }
    else {
        // Рисуем отладочный прямоугольник
        sf::RectangleShape debugRect(sf::Vector2f(WIDTH, HEIGHT));
        debugRect.setPosition(getPosition());
        debugRect.setFillColor(sf::Color::Yellow);  // Желтый = карта без спрайта
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setOutlineThickness(2);
        target.draw(debugRect);

        std::cout << "  ВНИМАНИЕ: Карта без спрайта!" << std::endl;
    }
}

bool Card::contains(sf::Vector2f point) const {
    // SFML 3: getLocalBounds возвращает sf::Rect
    if (!sprite) return false;
    return sprite->getGlobalBounds().contains(point);
}

//std::string Card::toString() const {
//    std::string suitStr;
//    switch (suit) {
//    case Suit::Hearts: suitStr = "ser"; break;
//    case Suit::Diamonds: suitStr = "bub"; break;
//    case Suit::Clubs: suitStr = "kr"; break;
//    case Suit::Spades: suitStr = "pik"; break;
//    }
//
//    std::string rankStr;
//    switch (rank) {
//    case Rank::Ace: rankStr = "A"; break;
//    case Rank::Jack: rankStr = "J"; break;
//    case Rank::Queen: rankStr = "Q"; break;
//    case Rank::King: rankStr = "K"; break;
//    default: rankStr = std::to_string(static_cast<int>(rank));
//    }
//
//    return rankStr + suitStr + (faceUp ? "up" : "dw");
//}