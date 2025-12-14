#include "Card.h"
#include <stdexcept>

std::shared_ptr<sf::Texture> Card::frontTexture = nullptr;
std::shared_ptr<sf::Texture> Card::backTexture = nullptr;

Card::Card(Suit s, Rank r) : suit(s), rank(r), faceUp(false), sprite(nullptr) {
    if (backTexture) {
        sprite = new sf::Sprite(*backTexture);

        // Масштабируем
        if (backTexture->getSize().x > 0 && backTexture->getSize().y > 0) {
            sprite->setScale(sf::Vector2f(
                static_cast<float>(WIDTH) / backTexture->getSize().x,
                static_cast<float>(HEIGHT) / backTexture->getSize().y
            ));
        }
    }
}

Card::~Card() {
    if (sprite != nullptr) {
        delete sprite;
        sprite = nullptr;
    }
}

Card::Card(const Card& other) noexcept
    : suit(other.suit),
    rank(other.rank),
    faceUp(other.faceUp),
    sprite(nullptr)  {

    // Создаем НОВЫЙ спрайт
    sprite = new sf::Sprite(faceUp ? *frontTexture: *backTexture);

    if (other.sprite) {
        // Копируем ВСЕ свойства спрайта
        *sprite = *(other.sprite);

        //// Убедимся, что текстура установлена
        //if (backTexture && !(sprite->getTexture())) {
        //    sprite->setTexture(*backTexture);
        //}
    }

    std::cout << "Вызван конструктор копирования Card" << std::endl;
}

// Оператор присваивания копированием
Card& Card::operator=(const Card& other) noexcept  {
    // Проверка на самоприсваивание
    if (this == &other) {
        return *this;
    }

    // 1. Копируем простые поля
    suit = other.suit;
    rank = other.rank;
    faceUp = other.faceUp;

    // 2. Обрабатываем спрайт (глубокое копирование)
    if (other.sprite) {
        // Если у нас уже есть спрайт
        if (sprite) {
            // Копируем состояние спрайта
            *sprite = *(other.sprite);

            //// Убедимся, что текстура установлена
            //if (backTexture && !sprite->getTexture()) {
            //    sprite->setTexture(*backTexture);
            //}
        }
        else {
            // Создаем новый спрайт
            sprite = new sf::Sprite(*(other.sprite));

            //// Убедимся, что текстура установлена
            //if (backTexture && !sprite->getTexture()) {
            //    sprite->setTexture(*backTexture);
            //}
        }
    }
    //else {
    //    // Если у other нет спрайта, удаляем свой
    //    if (sprite) {
    //        delete sprite;
    //        sprite = nullptr;
    //    }
    //}

    std::cout << "Вызван оператор присваивания Card" << std::endl;
    return *this;
}

Card::Card(Card&& other) noexcept
    : suit(other.suit), rank(other.rank), faceUp(other.faceUp), sprite(other.sprite) {
    other.sprite = nullptr; // забираем спрайт у другого объекта
}

// Оператор присваивания перемещением
Card& Card::operator=(Card&& other) noexcept {
    if (this != &other) {
        // Удаляем текущий спрайт
        if (sprite) {
            delete sprite;
        }

        // Перемещаем данные
        suit = other.suit;
        rank = other.rank;
        faceUp = other.faceUp;
        sprite = other.sprite;

        other.sprite = nullptr; // забираем спрайт
    }
    return *this;
}

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
    if (backTexture) {
        return true;
    }

    // Загружаем рубашку
    backTexture = std::make_shared<sf::Texture>();

    if (backTexture->loadFromFile(backPath)) {
        std::cout << "Текстура рубашки успешно загружена" << std::endl;

        // Создаем front texture (пока пустую)
        frontTexture = std::make_shared<sf::Texture>();

        return true;
    }
    else {
        std::cerr << "Ошибка: не удалось загрузить текстуру рубашки: " << backPath << std::endl;

        // Создаем простую текстуру для отладки
        // Можно создать цветной прямоугольник или оставить nullptr
        backTexture.reset();
        frontTexture.reset();

        return false;
    }
}

void Card::flip() {
    faceUp = !faceUp;
    if (sprite && frontTexture && backTexture) {
        if (faceUp) {
            // Пока frontTexture пустая, но можно загрузить позже
            // sprite->setTexture(*frontTexture, true);
        }
        else {
            sprite->setTexture(*backTexture, true);
        }
    }
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
    //    // Рисуем отладочный прямоугольник
    //    sf::RectangleShape debugRect(sf::Vector2f(WIDTH, HEIGHT));
    //    debugRect.setPosition(getPosition());
    //    debugRect.setFillColor(sf::Color::Yellow);  // Желтый = карта без спрайта
    //    debugRect.setOutlineColor(sf::Color::Red);
    //    debugRect.setOutlineThickness(2);
    //    target.draw(debugRect);

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