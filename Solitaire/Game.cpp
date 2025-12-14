// Game.cpp
#include "Game.h"
#include "Enums.h"
#include <iostream>

Game::Game() : stock({ 50, 50 }), gameWon(false) {
    std::cout << "Инициализация стопок" << std::endl;
    // Инициализируем стопки сбора
    foundations[0] = Foundation(Suit::Hearts, { 300, 50 });
    foundations[1] = Foundation(Suit::Diamonds, { 400, 50 });
    foundations[2] = Foundation(Suit::Clubs, { 500, 50 });
    foundations[3] = Foundation(Suit::Spades, { 600, 50 });

    // Игровые стопки
    for (int i = 0; i < 7; i++) {
        tableaus[i] = Tableau({ 50.0f + i * 110.0f, 250.0f });
    }
}

void Game::newGame() {
    // 1. Создаем и перемешиваем колоду
    deck = Deck();
    deck.initialize();
    deck.shuffle();
    std::cout << "Колода создана и перемешана" << std::endl;

    //std::string assetsPath = "assets\\cards";
    //if (!deck.loadCardTextures(assetsPath)) {
    //    std::cerr << "Внимание: не все текстуры загружены" << std::endl;
    //}


    // 2. Раздаем карты в игровые стопки
    setupTableaus();




    // 3. Остальные карты - в колоду для взятия
    while (deck.size() > 0) {
        //stock.addCard(deck.draw());

        Card card = deck.draw();
        stock.addCard(card);
    }

    stock.revealTopCard();
    //drawFromStock();
    //while (deck.size() > 0) {
    //    stock.addCard(drawFromStock());
    //}

    //if (!stock.isEmpty()) {
    //    stock.getCardsRef().back().setFaceUp(true);
    //}

    std::cout << "Сток" << std::endl;
    stock.output();
    std::cout << "Стопки заполнены" << std::endl;

    gameWon = false;
    score = 0;
}

void Game::setupTableaus() {
    // Классическая раздача косынки:
    // 1-я стопка: 1 карта (последняя открыта)
    // 2-я стопка: 2 карты (последняя открыта)
    // ...
    // 7-я стопка: 7 карт (последняя открыта)

    for (int i = 0; i < 7; i++) {
        int j;
        for (j = 0; j <= i; j++) {
            Card card = deck.draw();

            //// Последняя карта в стопке - открыта
            //if (j == i) {
            //    card.setFaceUp(true);
            //}

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

void Game::drawFromStock() {
    //if (!stock.isEmpty()) {
    //    Card drawnCard = stock.drawCard();
    //    drawnCard.setFaceUp(true);
    //    // Здесь нужно куда-то положить drawnCard
    //    // В косынке обычно есть Waste стопка, но для простоты...
    //    while (deck.size() > 0) {
    //        stock.addCard(drawnCard);
    //    }
    //}
}