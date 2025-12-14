#include "Card.h"
#include <stdexcept>

std::shared_ptr<sf::Texture> Card::backTexture = nullptr;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Card::frontTextureCache;

//std::shared_ptr<sf::Texture> Card::frontTexture = nullptr;
//std::shared_ptr<sf::Texture> Card::backTexture = nullptr;

Card::Card(Suit s, Rank r) : suit(s), rank(r), faceUp(false), sprite(nullptr) {
    //if (frontTexture != nullptr && backTexture != nullptr) {
    //    sprite = new sf::Sprite(faceUp? *frontTexture: *backTexture);

    //    // Масштабируем
    //    if (backTexture->getSize().x > 0 && backTexture->getSize().y > 0) {
    //        sprite->setScale(sf::Vector2f(
    //            static_cast<float>(WIDTH) / backTexture->getSize().x,
    //            static_cast<float>(HEIGHT) / backTexture->getSize().y
    //        ));
    //    }
    //}
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
    else {
        std::cerr << "Внимание: рубашка не загружена!" << std::endl;
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
    //sprite = new sf::Sprite(faceUp ? *frontTexture: *backTexture);

    //if (other.sprite) {
    //    // Копируем ВСЕ свойства спрайта
    //    *sprite = *(other.sprite);

    //    //// Убедимся, что текстура установлена
    //    //if (backTexture && !(sprite->getTexture())) {
    //    //    sprite->setTexture(*backTexture);
    //    //}
    //}
    if (faceUp) {
        // Если карта открыта, загружаем лицевую текстуру
        loadFrontTexture();
    }

    // Создаем НОВЫЙ спрайт
    if (faceUp) {
        std::string key = getTextureKey();
        auto it = frontTextureCache.find(key);
        if (it != frontTextureCache.end()) {
            sprite = new sf::Sprite(*it->second);
        }
        else {
            sprite = new sf::Sprite(*backTexture);
        }
    }
    else {
        sprite = new sf::Sprite(*backTexture);
    }

    if (other.sprite) {
        // Копируем свойства спрайта
        sprite->setPosition(other.sprite->getPosition());
        sprite->setScale(other.sprite->getScale());
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

std::string Card::getTextureFileName() const {
    std::string rankStr;
    switch (rank) {
    case Rank::Ace: rankStr = "Ace"; break;
    case Rank::Two: rankStr = "2"; break;
    case Rank::Three: rankStr = "3"; break;
    case Rank::Four: rankStr = "4"; break;
    case Rank::Five: rankStr = "5"; break;
    case Rank::Six: rankStr = "6"; break;
    case Rank::Seven: rankStr = "7"; break;
    case Rank::Eight: rankStr = "8"; break;
    case Rank::Nine: rankStr = "9"; break;
    case Rank::Ten: rankStr = "10"; break;
    case Rank::Jack: rankStr = "Jack"; break;
    case Rank::Queen: rankStr = "Queen"; break;
    case Rank::King: rankStr = "King"; break;
    }

    std::string suitStr;
    switch (suit) {
    case Suit::Hearts: suitStr = "Hearts"; break;
    case Suit::Diamonds: suitStr = "Diamonds"; break;
    case Suit::Clubs: suitStr = "Clubs"; break;
    case Suit::Spades: suitStr = "Spades"; break;
    }

    return rankStr + "_" + suitStr + ".jpg";
}

std::string Card::getTextureKey() const {
    return std::to_string(static_cast<int>(suit)) + "_" +
        std::to_string(static_cast<int>(rank));
}

bool Card::loadFrontTexture() {
    std::string key = getTextureKey();

    // Проверяем кэш
    auto it = frontTextureCache.find(key);
    if (it != frontTextureCache.end()) {
        return true; // Текстура уже загружена
    }

    // Загружаем из файла
    std::string fileName = getTextureFileName();
    std::string assetsPath = "assets/cards/cards_1/"; // Базовый путь

    // Можно настроить путь через настройки или параметр
    std::string fullPath = assetsPath + fileName;

    auto texture = std::make_shared<sf::Texture>();
    if (texture->loadFromFile(fullPath)) {
        frontTextureCache[key] = texture;
        std::cout << "Загружена текстура: " << fileName << std::endl;
        return true;
    }
    else {
        std::cerr << "Ошибка загрузки текстуры: " << fullPath << std::endl;
        return false;
    }
}

bool Card::loadBackTexture(const std::string& backPath) {
    if (backTexture) {
        return true; // Уже загружена
    }

    backTexture = std::make_shared<sf::Texture>();
    if (backTexture->loadFromFile(backPath)) {
        std::cout << "Рубашка загружена: " << backPath << std::endl;
        return true;
    }
    else {
        std::cerr << "Ошибка загрузки рубашки: " << backPath << std::endl;
        backTexture.reset();
        return false;
    }
}

void Card::clearTextureCache() {
    frontTextureCache.clear();
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

//bool Card::loadTextures(const std::string& frontPath, const std::string& backPath) {
//    if (backTexture) {
//        return true;
//    }
//
//    // Загружаем рубашку
//    backTexture = std::make_shared<sf::Texture>();
//
//    if (backTexture->loadFromFile(backPath)) {
//        std::cout << "Текстура рубашки успешно загружена" << std::endl;
//
//        // Создаем front texture (пока пустую)
//        frontTexture = std::make_shared<sf::Texture>();
//
//        return true;
//    }
//    else {
//        std::cerr << "Ошибка: не удалось загрузить текстуру рубашки: " << backPath << std::endl;
//
//        // Создаем простую текстуру для отладки
//        // Можно создать цветной прямоугольник или оставить nullptr
//        backTexture.reset();
//        frontTexture.reset();
//
//        return false;
//    }
//}

void Card::flip() {
    //faceUp = !faceUp;
    //if (sprite && frontTexture && backTexture) {
    //    if (faceUp) {
    //        std::cout << "Переворачиваю лицом вверх" << std::endl;
    //        sprite->setTexture(*frontTexture, true);
    //    }
    //    else {
    //        std::cout << "Переворачиваю рубашкой вверх" << std::endl;
    //        sprite->setTexture(*backTexture, true);
    //    }
    //}
    if (faceUp == !faceUp) return; // Уже в нужном состоянии

    faceUp = !faceUp;

    if (!sprite) {
        std::cerr << "Нет спрайта для переворота!" << std::endl;
        return;
    }

    if (faceUp) {
        // Переворачиваем на лицевую сторону
        // 1. Загружаем текстуру (если еще не загружена)
        if (!loadFrontTexture()) {
            std::cerr << "Не удалось загрузить лицевую текстуру!" << std::endl;
            faceUp = false; // Отменяем переворот
            return;
        }

        // 2. Устанавливаем текстуру
        std::string key = getTextureKey();
        auto it = frontTextureCache.find(key);
        if (it != frontTextureCache.end()) {
            sprite->setTexture(*it->second, true);

            // Масштабируем под размер карты
            sf::Vector2u texSize = it->second->getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sprite->setScale(sf::Vector2f(
                    static_cast<float>(WIDTH) / texSize.x,
                    static_cast<float>(HEIGHT) / texSize.y
                ));
            }
        }
    }
    else {
        // Переворачиваем на рубашку
        if (backTexture) {
            sprite->setTexture(*backTexture, true);

            // Масштабируем
            sf::Vector2u texSize = backTexture->getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sprite->setScale(sf::Vector2f(
                    static_cast<float>(WIDTH) / texSize.x,
                    static_cast<float>(HEIGHT) / texSize.y
                ));
            }
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
    // SFML 3: getLocalBounds возвращает sf::Rect contains - содержит
    if (!sprite) return false;
    return sprite->getGlobalBounds().contains(point);
}

