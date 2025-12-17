#pragma once
#include <array>
#include <stack>
#include <memory>
#include "Deck.h"
#include "Tableau.h"
#include "Foundation.h"
#include "Stock.h"
#include "Waste.h"

class Game {
private:
    Deck deck;
    Stock stock;
    Waste waste;
    std::array<Tableau, 7> tableaus;
    std::array<Foundation, 4> foundations;

    sf::Clock gameClock; // Таймер игры
    sf::Time gameTime;   // Общее время игры
    bool isGameActive;   // Активна ли игра
    bool timerRunning;

    std::mt19937 rng;

    int stockDrawCount;
    int stockCycleIndex;

    bool gameWon;
    int score;

public:
    Game();

    void newGame();
    bool moveCard(Pile& from, Pile& to, int cardIndex = -1);

    void drawFromStock();
    void resetStock();
    bool canDrawFromStock() const;
    bool canResetStock() const;

    void startTimer();
    void pauseTimer();
    void resumeTimer();
    sf::Time getGameTime() const;
    std::string getFormattedTime() const;

    void clear();

    const Stock& getStock() const { return stock; }
    const auto& getTableaus() const { return tableaus; }
    const Waste& getWaste() const { return waste; }
    const auto& getFoundations() const { return foundations; }

private:
    void setupTableaus();
};