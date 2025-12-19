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
        tableaus[i] = Tableau({ 100.0f + i * 110.0f, 250.0f + 35 });  
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

std::string Game::getFormattedTimeWithMs() const {
    sf::Time totalTime = getGameTime();
    int totalMilliseconds = static_cast<int>(totalTime.asMilliseconds());

    int hours = totalMilliseconds / 3600000;
    int minutes = (totalMilliseconds % 3600000) / 60000;
    int seconds = (totalMilliseconds % 60000) / 1000;
    int milliseconds = totalMilliseconds % 1000;

    if (hours > 0) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d.%03d",
            hours, minutes, seconds, milliseconds);
        return std::string(buffer);
    }
    else {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%02d:%02d.%03d",
            minutes, seconds, milliseconds);
        return std::string(buffer);
    }
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
    stock.clear();
    waste.clear();

    for (auto& tableau : tableaus) {
        tableau.clear();
    }

    for (auto& foundation : foundations) {
        foundation.clear();
    }

    deck = Deck();

    gameWon = false;
    score = 0;
}

void Game::newGame() {
    clear();
    startTimer();

    deck.initialize();
    deck.shuffle();

    setupTableaus();

    while (deck.size() > 0) {
        Card card = deck.draw();
        stock.addCard(card);
    }

    gameWon = false;
    score = 0;
}

void Game::resetStock() {
    while (!waste.isEmpty()) {
        Card card = waste.removeTopCard();
        card.setFaceUp(false);  
        stock.addCard(card);
    }
}

void Game::drawFromStock() {
    std::vector<Card> allCards;

    while (!waste.isEmpty()) {
        Card card = waste.removeTopCard();
        card.setFaceUp(false);
        allCards.push_back(card);
    }

    while (!stock.isEmpty()) {
        allCards.push_back(stock.removeTopCard());
    }

    if (allCards.empty()) return;

    int cardsToShow = std::min(3, static_cast<int>(allCards.size()));

    std::shuffle(allCards.begin(), allCards.end(), rng);

    std::vector<Card> cardsForWaste;
    for (int i = 0; i < cardsToShow; i++) {
        Card card = allCards[i];
        card.setFaceUp(true);
        cardsForWaste.push_back(card);
    }

    for (int i = cardsToShow; i < allCards.size(); i++) {
        Card card = allCards[i];
        card.setFaceUp(false);
        stock.addCard(card);
    }

    for (int i = static_cast<int>(cardsForWaste.size()) - 1; i >= 0; i--) {
        waste.addCard(cardsForWaste[i]);
    }
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
        tableaus[i].revealTopCard();
    }
}

bool Game::moveCard(Pile& from, Pile& to, int cardIndex) {
    if (from.isEmpty()) return false;

    Card cardToMove = from.getTopCard();

    if (!to.canAddCard(cardToMove)) {
        return false;
    }

    Card movedCard = from.removeTopCard();
    to.addCard(movedCard);

    if (!from.isEmpty() && !from.getTopCard().isFaceUp()) {
        from.getTopCard().setFaceUp(true);
    }

    return true;
}
