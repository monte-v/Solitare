#include "Foundation.h"

std::array<std::shared_ptr<sf::Texture>, 4> Foundation::suitTextures = {
    nullptr, nullptr, nullptr, nullptr
};

Foundation::Foundation() : Foundation(Suit::Hearts, { 0, 0 }) {}

Foundation::Foundation(Suit s, sf::Vector2f pos)
    : Pile(pos, 0.0f), suit(s) {
} 

Foundation::Foundation(const Foundation& other)
    : Pile(other), suit(other.suit) {
}

Foundation& Foundation::operator=(const Foundation& other) {
    if (this != &other) {
        Pile::operator=(other);
        suit = other.suit;
    }
    return *this;
}

Foundation::Foundation(Foundation&& other) noexcept
    : Pile(std::move(other)), suit(std::move(other.suit)) {
}

Foundation& Foundation::operator=(Foundation&& other) noexcept {
    if (this != &other) {
        Pile::operator=(std::move(other));
        suit = std::move(other.suit);
    }
    return *this;
}

bool Foundation::canAddCard(const Card& card) const {
    if (card.getSuit() != suit) {
        return false;
    }

    if (cards.empty()) {
        bool canAdd = card.getRank() == Rank::Ace;
        return canAdd;
    }

    const Card& topCard = getTopCard();

    bool canAdd = static_cast<int>(card.getRank()) ==
        static_cast<int>(topCard.getRank()) + 1;

    return canAdd;
}

bool Foundation::isComplete() const {
    return cards.size() == 13;
}

bool Foundation::loadTextures(const std::string& basePath) {
    std::array<std::string, 4> fileNames = {
        "Hearts.png", "Diamonds.png", "Clubs.png", "Spades.png"
    };

    bool allLoaded = true;

    for (int i = 0; i < 4; i++) {
        if (!suitTextures[i]) {
            std::string fullPath = basePath + fileNames[i];
            suitTextures[i] = std::make_shared<sf::Texture>();

            if (!suitTextures[i]->loadFromFile(fullPath)) {
            //    std::cout << "Текстура " << fileNames[i] << " загружена" << std::endl;
            //}
            //else {
                std::cerr << "Ошибка загрузки: " << fullPath << std::endl;
                suitTextures[i].reset();
                allLoaded = false;
            }
        }
    }

    return allLoaded;
}

void Foundation::draw(sf::RenderTarget& target) const {
    Pile::draw(target);

    if (cards.empty()) {
        int suitIndex = static_cast<int>(suit);

        if (suitIndex >= 0 && suitIndex < 4 && suitTextures[suitIndex]) {
            sf::Sprite suitSprite(*suitTextures[suitIndex]);
            suitSprite.setPosition(position);

            sf::Vector2u texSize = suitTextures[suitIndex]->getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                suitSprite.setScale(sf::Vector2f(
                    static_cast<float>(Card::WIDTH) / texSize.x,
                    static_cast<float>(Card::HEIGHT) / texSize.y
                ));
            }

            target.draw(suitSprite);
        }
    }
}
