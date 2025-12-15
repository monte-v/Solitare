#pragma once
#include <array>
#include <stack>
#include <memory>
#include "Deck.h"
#include "Tableau.h"
#include "Foundation.h"
#include "Stock.h"

// Структура для сохранения состояния хода
struct MoveRecord {
    std::shared_ptr<Pile> fromPile;
    std::shared_ptr<Pile> toPile;
    int cardIndex;
    Card card;
};

class Game {
private:
    Deck deck;
    Stock stock;
    std::array<Tableau, 7> tableaus;
    std::array<Foundation, 4> foundations;

    std::stack<MoveRecord> moveHistory;

    bool gameWon;
    int score;

public:
    Game();

    void newGame();
    bool moveCard(Pile& from, Pile& to, int cardIndex = -1);

    void clear();

    // Система отмены (ваша фишка)
    //bool undoLastMove();
    //void saveMove(const MoveRecord& record);

    // Проверки
    //bool isGameWon() const;
    //bool isMoveValid(const Card& card, const Pile& to) const;

    const Stock& getStock() const { return stock; }
    const auto& getTableaus() const { return tableaus; }
    const auto& getFoundations() const { return foundations; }
    int getScore() const { return score; }

    void addScore(int points) { score += points; }

private:
    void setupTableaus();
};