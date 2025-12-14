#include "Deck.h"
#include <filesystem>  // SFML 3 рекомендует использовать filesystem

namespace fs = std::filesystem;

Deck::Deck() : rng(std::random_device{}()) {}

void Deck::output() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". ";
        cards[i].output();
    }
}

void Deck::initialize() {
    cards.clear();
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            cards.emplace_back(
                static_cast<Suit>(s),
                static_cast<Rank>(r)
            );
        }
    }
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::draw() {
    if (cards.empty()) {
        throw std::runtime_error("Deck is empty");
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::addCard(Card card) {
    cards.push_back(card);
}

bool Deck::loadCardTextures(const std::string& directory) {
    
    std::cout << "«агрузка рубашек дл€ всех карт..." << std::endl;

    // “олько рубашка
    std::string backPath = directory + "\\cards_1\\shirt.png";
    std::string frontPath = backPath;

    for (auto& card : cards) {
        if (!card.loadTextures(frontPath, backPath)) {
            std::cerr << "Ќе удалось загрузить текстуры дл€ одной карты" << std::endl;
            return false;
        }
    }

    return true;
}