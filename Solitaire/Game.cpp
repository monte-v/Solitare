// Game.cpp
#include "Game.h"
#include "Enums.h"
#include <iostream>

Game::Game() : stock({ 100, 50 + 35 }), waste({ 220, 50 + 35 }), gameWon(false), score(0), 
    stockDrawCount(3), stockCycleIndex(0), rng(std::random_device{}()), 
    isGameActive(false), timerRunning(false) {
    foundations[0] = Foundation(Suit::Hearts, { 430, 50 + 35 });
    foundations[1] = Foundation(Suit::Diamonds, { 540, 50 + 35 });
    foundations[2] = Foundation(Suit::Clubs, { 650, 50 + 35 });
    foundations[3] = Foundation(Suit::Spades, { 760, 50 + 35 });

    for (int i = 0; i < 7; i++) {
        tableaus[i] = Tableau({ 100.0f + i * 110.0f, 250.0f + 35 });  // +35
    }
}

void Game::startTimer() {
    gameClock.restart();
    isGameActive = true;
}

void Game::pauseTimer() {
    if (isGameActive) {
        gameTime += gameClock.getElapsedTime();
        isGameActive = false;
    }
}

void Game::resumeTimer() {
    if (!isGameActive) {
        gameClock.restart();
        isGameActive = true;
    }
}

sf::Time Game::getGameTime() const {
    if (isGameActive) {
        return gameTime + gameClock.getElapsedTime();
    }
    return gameTime;
}

std::string Game::getFormattedTime() const {
    sf::Time totalTime = getGameTime();
    int totalSeconds = static_cast<int>(totalTime.asSeconds());

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (hours > 0) {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
        return std::string(buffer);
    }
    else {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
        return std::string(buffer);
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

    std::cout << "Текущая игра очищена" << std::endl;
}

void Game::newGame() {
    clear();
    startTimer();

    deck.initialize();
    deck.shuffle();
    std::cout << "Колода создана и перемешана" << std::endl;

    setupTableaus();

    while (deck.size() > 0) {
        Card card = deck.draw();
        stock.addCard(card);
    }

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
    // Собираем все карты из waste обратно в stock
    std::vector<Card> allCards;

    // Сначала переносим waste в общий пул
    while (!waste.isEmpty()) {
        Card card = waste.removeTopCard();
        card.setFaceUp(false);
        allCards.push_back(card);
    }

    // Затем добавляем карты из stock
    while (!stock.isEmpty()) {
        allCards.push_back(stock.removeTopCard());
    }

    if (allCards.empty()) return;

    // Если карт меньше 3, просто показываем все
    int cardsToShow = std::min(3, static_cast<int>(allCards.size()));

    // Перемешиваем все карты
    std::shuffle(allCards.begin(), allCards.end(), rng);

    // Берем случайные cardsToShow карт для показа
    std::vector<Card> cardsForWaste;
    for (int i = 0; i < cardsToShow; i++) {
        Card card = allCards[i];
        card.setFaceUp(true);
        cardsForWaste.push_back(card);
    }

    // Остальные карты остаются в stock
    for (int i = cardsToShow; i < allCards.size(); i++) {
        Card card = allCards[i];
        card.setFaceUp(false);
        stock.addCard(card);
    }

    // Добавляем отобранные карты в waste (в обратном порядке для правильного отображения)
    for (int i = static_cast<int>(cardsForWaste.size()) - 1; i >= 0; i--) {
        waste.addCard(cardsForWaste[i]);
    }

    //std::cout << "Stock: " << stock.getCardCount() << " карт" << std::endl;
    //std::cout << "Waste: " << waste.getCardCount() << " карт" << std::endl;
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
