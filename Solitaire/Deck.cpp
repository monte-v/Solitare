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

//bool Deck::loadCardTextures(const std::string& directory) {
    
    //std::cout << "«агрузка рубашек дл€ всех карт..." << std::endl;

    //// “олько рубашка
    //std::string backPath = directory + "\\cards_1\\shirt.png";

    //for (auto& card : cards) {
    //    // ‘ормируем путь к текстуре лицевой стороны
    //    std::string rankName;
    //    switch (card.getRank()) {
    //    case Rank::Ace: rankName = "Ace"; break;
    //    case Rank::Two: rankName = "2"; break;
    //    case Rank::Three: rankName = "3"; break;
    //    case Rank::Four: rankName = "4"; break;
    //    case Rank::Five: rankName = "5"; break;
    //    case Rank::Six: rankName = "6"; break;
    //    case Rank::Seven: rankName = "7"; break;
    //    case Rank::Eight: rankName = "8"; break;
    //    case Rank::Nine: rankName = "9"; break;
    //    case Rank::Ten: rankName = "10"; break;
    //    case Rank::Jack: rankName = "Jack"; break;
    //    case Rank::Queen: rankName = "Queen"; break;
    //    case Rank::King: rankName = "King"; break;
    //    }

    //    std::string suitName;
    //    switch (card.getSuit()) {
    //    case Suit::Hearts: suitName = "Hearts"; break;
    //    case Suit::Diamonds: suitName = "Diamonds"; break;
    //    case Suit::Clubs: suitName = "Clubs"; break;
    //    case Suit::Spades: suitName = "Spades"; break;
    //    }

    //    std::string frontFileName = rankName + "_" + suitName + ".jpg";
    //    std::string frontPath = directory + "\\cards_1\\" + frontFileName;

    //    std::cout << "«агрузка текстуры дл€: " << rankName << " " << suitName
    //        << " из: " << frontPath << std::endl;

    //    if (!card.loadTextures(frontPath, backPath)) {
    //        std::cerr << "Ќе удалось загрузить текстуры дл€ карты: "
    //            << rankName << " " << suitName << std::endl;
    //        // ћожно продолжить без этой текстуры или вернуть false
    //        return false;
    //    }
    //}


    //std::string frontPath = backPath;

    //for (auto& card : cards) {
    //    if (!card.loadTextures(frontPath, backPath)) {
    //        std::cerr << "Ќе удалось загрузить текстуры дл€ одной карты" << std::endl;
    //        return false;
    //    }
    //}

//    return true;
//}