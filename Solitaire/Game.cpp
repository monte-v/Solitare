// Game.cpp
#include "Game.h"
#include "Enums.h"
#include <iostream>

Game::Game() : stock({ 100, 50 + 35 }), waste({ 220, 50 + 35 }), gameWon(false), score(0), stockDrawCount(3), stockCycleIndex(0) {
    std::cout << "Инициализация стопок" << std::endl;
    foundations[0] = Foundation(Suit::Hearts, { 430, 50 + 35 });
    foundations[1] = Foundation(Suit::Diamonds, { 540, 50 + 35 });
    foundations[2] = Foundation(Suit::Clubs, { 650, 50 + 35 });
    foundations[3] = Foundation(Suit::Spades, { 760, 50 + 35 });

    for (int i = 0; i < 7; i++) {
        tableaus[i] = Tableau({ 100.0f + i * 110.0f, 250.0f + 35 });  // +35
    }
}

void Game::clear() {
    // Очищаем все стопки
    stock.clear();
    waste.clear();

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

    std::cout << "Сток" << std::endl;
    stock.output();
    std::cout << "Стопки заполнены" << std::endl;

    gameWon = false;
    score = 0;
}

void Game::resetStock() {
    // Возвращаем все карты из Waste обратно в Stock
    while (!waste.isEmpty()) {
        Card card = waste.removeTopCard();
        card.setFaceUp(false);  // В Stock карты рубашкой вверх
        stock.addCard(card);
    }

    // Если нужно, можно перевернуть верхнюю карту
    // Но в Stock обычно показывают рубашку, так что оставляем false
}

void Game::drawFromStock() {
    // Берем до 3 карт из Stock (или сколько осталось)
    //int cardsToDraw = std::min(stockDrawCount, static_cast<int>(stock.getCardCount()));

    //while (!waste.isEmpty()) {
    //    Card card = waste.removeTopCard();
    //    card.setFaceUp(false);  // В Stock карты рубашкой вверх
    //    stock.addCard(card);
    //}
    //int cardsToDraw = std::min(stockDrawCount, static_cast<int>(stock.getCardCount()));
    //std::cout << "Берём " << cardsToDraw << " новых карт из Stock" << std::endl;

    //for (int i = 0; i < cardsToDraw; i++) {
    //    if (!stock.isEmpty()) {
    //        Card card = stock.removeTopCard();
    //        card.setFaceUp(true);  // В Waste карты всегда открыты
    //        waste.addCard(card);
    //    }
    //}
    // Если Stock пуст, а Waste не пуст - начинаем цикл заново





    //if (stock.isEmpty() && !waste.isEmpty()) {
    //    std::cout << "Начинаем цикл заново..." << std::endl;

    //    // Перемещаем ВСЕ карты из Waste в Stock
    //    while (!waste.isEmpty()) {
    //        Card card = waste.removeTopCard();
    //        card.setFaceUp(false);  // В Stock карты закрыты
    //        stock.addCard(card);
    //    }

    //    std::cout << "Все карты вернулись в Stock: " << stock.getCardCount() << " карт" << std::endl;

    //    // Если после этого Stock все еще пуст (не должно быть)
    //    if (stock.isEmpty()) {
    //        return;
    //    }
    //}

    //// Теперь берем 3 карты из Stock в Waste
    //// Но! Карты из Waste должны вернуться в Stock в правильном порядке

    //// Временное хранилище для текущих карт Waste
    //std::vector<Card> currentWasteCards;
    //while (!waste.isEmpty()) {
    //    currentWasteCards.push_back(waste.removeTopCard());
    //}

    //// Добавляем эти карты обратно в Stock (в конец)
    //for (auto& card : currentWasteCards) {
    //    card.setFaceUp(false);
    //    stock.addCard(card);
    //}

    //// Теперь берем 3 новые карты из Stock
    //int cardsToTake = std::min(3, static_cast<int>(stock.getCardCount()));
    //std::cout << "Берём " << cardsToTake << " карт из Stock" << std::endl;

    //for (int i = 0; i < cardsToTake; i++) {
    //    if (!stock.isEmpty()) {
    //        Card card = stock.removeTopCard();
    //        card.setFaceUp(true);  // В Waste карты открыты
    //        waste.addCard(card);
    //    }
    //}

    //std::cout << "После drawFromStock:" << std::endl;
    //std::cout << "  Stock: " << stock.getCardCount() << " карт" << std::endl;
    //std::cout << "  Waste: " << waste.getCardCount() << " карт" << std::endl;

    //// Вывести верхнюю карту Waste для отладки
    //if (!waste.isEmpty()) {
    //    std::cout << "Верхняя карта Waste: ";
    //    waste.getTopCard().output();
    //}

    std::vector<Card> allCards;

    while (!waste.isEmpty()) {
        allCards.insert(allCards.begin(), waste.removeTopCard());  // В начало
    }

    while (!stock.isEmpty()) {
        allCards.push_back(stock.removeTopCard());  // В конец
    }

    if (allCards.empty()) return;

    // ПРОСТОЙ АЛГОРИТМ: 
    // 1. Берем первые 3 карты и кладем их В КОНЕЦ вектора
    // 2. Следующие 3 карты будут показаны в Waste

    // Сдвигаем первые 3 карты в конец
    if (allCards.size() >= 3) {
        // Берем первые 3 карты
        std::vector<Card> firstThree;
        for (int i = 0; i < 3; i++) {
            firstThree.push_back(allCards[0]);
            allCards.erase(allCards.begin());
        }

        // Добавляем их в конец
        for (const auto& card : firstThree) {
            allCards.push_back(card);
        }
    }

    // Теперь берем первые 3 карты (или сколько есть) для Waste
    int cardsForWaste = std::min(3, static_cast<int>(allCards.size()));

    for (int i = 0; i < cardsForWaste; i++) {
        Card card = allCards[i];
        card.setFaceUp(true);
        waste.addCard(card);
    }

    // Остальные карты в Stock
    for (int i = cardsForWaste; i < allCards.size(); i++) {
        Card card = allCards[i];
        card.setFaceUp(false);
        stock.addCard(card);
    }

    std::cout << "Stock: " << stock.getCardCount() << " карт" << std::endl;
    std::cout << "Waste: " << waste.getCardCount() << " карт" << std::endl;


}

bool Game::canDrawFromStock() const {
    return !stock.isEmpty() || !waste.isEmpty();
}

bool Game::canResetStock() const {
    return stock.isEmpty() && !waste.isEmpty();
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
