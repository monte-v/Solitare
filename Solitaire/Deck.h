#pragma once
#include <vector>
#include <random>
#include <memory>
#include <iostream>
#include "Card.h"


class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;

public:
    Deck();

    void output() const;

    void initialize();
    void shuffle();
    Card draw();
    void addCard(Card card);

    bool isEmpty() const { return cards.empty(); }
    size_t size() const { return cards.size(); }

    // Загрузка текстур для всех карт (оптимизация)
    bool loadCardTextures(const std::string& directory);
};