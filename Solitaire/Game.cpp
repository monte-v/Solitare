// Game.cpp
#include "Game.h"
#include "Enums.h"
#include <iostream>

Game::Game() : stock({ 50, 50 + 35 }), gameWon(false), score(0) {
    std::cout << "Инициализация стопок" << std::endl;
    //foundations[0] = Foundation(Suit::Hearts, { 300, 50 });
    //foundations[1] = Foundation(Suit::Diamonds, { 400, 50 });
    //foundations[2] = Foundation(Suit::Clubs, { 500, 50 });
    //foundations[3] = Foundation(Suit::Spades, { 600, 50 });

    //for (int i = 0; i < 7; i++) {
    //    tableaus[i] = Tableau({ 50.0f + i * 110.0f, 250.0f });
    //}

    foundations[0] = Foundation(Suit::Hearts, { 300, 50 + 35 });
    foundations[1] = Foundation(Suit::Diamonds, { 400, 50 + 35 });
    foundations[2] = Foundation(Suit::Clubs, { 500, 50 + 35 });
    foundations[3] = Foundation(Suit::Spades, { 600, 50 + 35 });

    for (int i = 0; i < 7; i++) {
        tableaus[i] = Tableau({ 50.0f + i * 110.0f, 250.0f + 35 });  // +35
    }
}

void Game::clear() {
    // Очищаем все стопки
    stock.clear();

    for (auto& tableau : tableaus) {
        tableau.clear();
    }

    for (auto& foundation : foundations) {
        foundation.clear();
    }

    // Сбрасываем колоду
    deck = Deck();

    // Сбрасываем флаги
    gameWon = false;
    score = 0;

    // Очищаем историю ходов
    while (!moveHistory.empty()) {
        moveHistory.pop();
    }

    std::cout << "Текущая игра очищена" << std::endl;
}

void Game::newGame() {
    clear();

    deck.initialize();
    deck.shuffle();
    std::cout << "Колода создана и перемешана" << std::endl;

    setupTableaus();

    while (deck.size() > 0) {
        Card card = deck.draw();
        stock.addCard(card);
    }

    stock.revealTopCard();

    std::cout << "Сток" << std::endl;
    stock.output();
    std::cout << "Стопки заполнены" << std::endl;

    gameWon = false;
    score = 0;
}

void Game::setupTableaus() {
    for (int i = 0; i < 7; i++) {
        int j;
        for (j = 0; j <= i; j++) {
            Card card = deck.draw();
            tableaus[i].addCard(card);
        }
        std::cout << "Переворачиваю карту из стопки " << i << " tableau" << std::endl;
        tableaus[i].revealTopCard();
        if (tableaus[i].getTopCard().isFaceUp()) { std::cout << "Карта перевернута" << std::endl; };
        std::cout << "Стопка " << i << " содержит "<< j << " карт" << std::endl;
    }
}

bool Game::moveCard(Pile& from, Pile& to, int cardIndex) {
    if (from.isEmpty()) return false;

    // Берем карту (или несколько, если из Tableau)
    Card cardToMove = from.getTopCard();

    // Проверяем, можно ли положить
    if (!to.canAddCard(cardToMove)) {
        return false;
    }

    // Перемещаем
    Card movedCard = from.removeTopCard();
    to.addCard(movedCard);

    // Если освободили карту в Tableau - открываем ее
    if (!from.isEmpty() && !from.getTopCard().isFaceUp()) {
        from.getTopCard().setFaceUp(true);
    }

    return true;
}
