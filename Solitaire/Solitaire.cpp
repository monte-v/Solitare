#include "Solitaire.h"
#include <iostream>

Solitaire::Solitaire() : window(sf::VideoMode({ 1000, 800 }), "Косынка", sf::Style::Titlebar | sf::Style::Close),
    sourcePile(nullptr), isDragging(false), startDragMousePos(0, 0), gameStarted(false), highlightEffect(), gameWon(false) {
    if (!initialize()) {
        std::cerr << "Не удалось инициализировать приложение" << std::endl;
        window.close();
    }
}

bool Solitaire::initialize() {
    if (!loadResources()) {
        return false;
    }

    winScreen.setSize(1000, 800);
    winScreen.setPosition(0, 0);

    menuBar.setSize(sf::Vector2f(1000, 35));
    menuBar.setPosition(sf::Vector2f(0, 0));

    startMenu.setRecordsManager(&recordsManager);

    menuBar.setOnHintCallback([this]() {
        if (gameStarted) {
            findAndShowHint();
        }
    });

    startMenu.setOnStart([this]() {
        startGame();
    });
    startMenu.setOnRecords([this]() {
        startMenu.showRecordsTable();
        startMenu.updateRecordsTable();
    });
    startMenu.setOnHelp([this]() {
        helpWindow.open();
    });
    startMenu.setOnExit([this]() {
        window.close();
    });

    startMenu.show();

    return true;
}

bool Solitaire::loadIcon(std::string path) {
    sf::Image icon;

    if (icon.loadFromFile(path)) {
        window.setIcon(icon);
        return true;
    }

    return false;
}

void Solitaire::checkAndAddRecord() {
    sf::Time gameTime = game.getGameTime();
    std::string formattedTime = recordsManager.formatTimeWithMs(gameTime);

    recordsManager.addRecord(gameTime, formattedTime);

    startMenu.updateRecordsTable();
}

void Solitaire::findAndShowHint() {
    highlightEffect.clear();

    std::vector<Pile*> allPiles;

    if (!game.getWaste().isEmpty()) {
        allPiles.push_back(&const_cast<Waste&>(game.getWaste()));
    }

    for (int i = 0; i < 7; i++) {
        if (!game.getTableaus()[i].isEmpty()) {
            allPiles.push_back(&const_cast<Tableau&>(game.getTableaus()[i]));
        }
    }

    for (Pile* sourcePile : allPiles) {
        if (sourcePile->isEmpty()) continue;

        const Card& sourceCard = sourcePile->getTopCard();

        for (int i = 0; i < 4; i++) {
            Foundation& foundation = const_cast<Foundation&>(game.getFoundations()[i]);
            if (foundation.canAddCard(sourceCard)) {
                sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                highlightEffect.addHighlight(sourcePos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                sf::Vector2f targetPos = foundation.getPosition();
                highlightEffect.addHighlight(targetPos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                return; 
            }
        }

        for (int i = 0; i < 7; i++) {
            Tableau& tableau = const_cast<Tableau&>(game.getTableaus()[i]);
            if (&tableau != sourcePile && tableau.canAddCard(sourceCard)) {
                sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                highlightEffect.addHighlight(sourcePos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                sf::Vector2f targetPos;
                if (tableau.isEmpty()) {
                    targetPos = tableau.getPosition();
                }
                else {
                    targetPos = tableau.getCardPosition(static_cast<int>(tableau.getCardCount()));
                }
                highlightEffect.addHighlight(targetPos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                return; 
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        Tableau& tableau = const_cast<Tableau&>(game.getTableaus()[i]);
        if (tableau.isEmpty()) {
            for (Pile* sourcePile : allPiles) {
                if (sourcePile->isEmpty()) continue;

                const Card& sourceCard = sourcePile->getTopCard();
                if (sourceCard.getRank() == Rank::King) {
                    sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                    highlightEffect.addHighlight(sourcePos,
                        sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                    sf::Vector2f targetPos = tableau.getPosition();
                    highlightEffect.addHighlight(targetPos,
                        sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                    return; 
                }
            }
        }
    }

    if (game.canDrawFromStock()) {
        sf::Vector2f stockPos = game.getStock().getPosition();
        highlightEffect.addHighlight(stockPos,
            sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);
    }
}

void Solitaire::startGame() {
    try {
        game.newGame();
        gameStarted = true;
        startMenu.hide();
        game.startTimer();  
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
    }
}

bool Solitaire::loadResources() {
    if (!loadIcon("assets/icon.png")) {
        std::cerr << "Используется стандартная иконка окна" << std::endl;
    }
    if (!startMenu.initialize("assets/fonts/arial.ttf", "assets/2.jpeg")) {
        std::cerr << "Не удалось инициализировать стартовое меню" << std::endl;
    }
    if (!winScreen.loadFont("assets/fonts/arial.ttf")) {
        std::cerr << "Шрифт для WinScreen не загружен" << std::endl;
    }
    if (!menuBar.loadFont("assets/fonts/arial.ttf")) {
        std::cerr << "Шрифт для меню не загружен" << std::endl;
    }
    if (!Card::loadBackTexture("assets/cards/cards_1/shirt.jpg")) {
        std::cerr << "Предупреждение: рубашка не загружена" << std::endl;
    }
    if (!Pile::loadEmptyPileTexture("assets/cards/Pile_1/Pile1.jpg")) {
        std::cerr << "Предупреждение: текстура пустой стопки не загружена" << std::endl;
    }
    if (!Foundation::loadTextures("assets/cards/Pile_1/")) {
        std::cerr << "Текстура Foundation не загружена" << std::endl;
    }

    return true; 
}

void Solitaire::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        processEvents();

        sf::Time deltaTime = clock.restart();
        update(deltaTime);

        render();
        sf::sleep(sf::milliseconds(16));
    }
}

void Solitaire::update(sf::Time deltaTime) {
    highlightEffect.update(deltaTime);

    if (gameStarted && !gameWon && !winScreen.isVisible() && !startMenu.isVisible()) {
        bool allFoundationsComplete = true;
        for (int i = 0; i < 4; i++) {
            if (!game.getFoundations()[i].isComplete()) {
                allFoundationsComplete = false;
                break;
            }
        }

        if (allFoundationsComplete) {
            gameWon = true;
            std::string gameTime = game.getFormattedTime();
            checkAndAddRecord();
            std::string detailedTime = recordsManager.formatTimeForDisplay(game.getGameTime());
            winScreen.show(gameTime);
        }
    }
}

void Solitaire::processEvents() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (!event) continue;

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                if (startMenu.isShowingRecords()) {
                    startMenu.hideRecordsTable();
                }
                else if (gameStarted && !startMenu.isVisible()) {
                    startMenu.show();
                    game.pauseTimer(); 
                }
                else if (gameStarted && startMenu.isVisible()) {
                    startMenu.hide();
                    game.resumeTimer(); 
                }
            }
        }
        else if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(mousePressed->position.x, mousePressed->position.y)
            );

            if (winScreen.isVisible()) {
                if (winScreen.handleClick(mousePos.x, mousePos.y)) {
                    game.newGame();
                    gameWon = false;
                    return;
                }
            }

            if (startMenu.isVisible()) {
                startMenu.handleClick(mousePos);
            }
            else {
                if (mousePos.y <= 35) {
                    menuBar.handleClick(mousePos);
                }
                else {
                    handleMousePressed(*mousePressed);
                }
            }
        }
        else if (auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (gameStarted && !startMenu.isVisible() && !helpWindow.isOpen()) {
                handleMouseReleased(*mouseReleased);
            }
        }
        else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            if (gameStarted && isDragging && !startMenu.isVisible() && !helpWindow.isOpen()) {
                handleMouseMoved(*mouseMoved);
            }
        }
    }

    if (helpWindow.isOpen()) {
        helpWindow.processEvents();
    }
}

void Solitaire::handleMousePressed(const sf::Event::MouseButtonPressed& event) {
    if (event.button == sf::Mouse::Button::Left && !isDragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.position.x, event.position.y));

        if (mousePos.y > 35) {
            Pile* pile = getPileAt(mousePos);

            if (pile) {
                if (dynamic_cast<Stock*>(pile)) {
                    game.drawFromStock();
                    return;
                }
                if (!pile->isEmpty()) {
                    int cardIndex = pile->getCardIndexAt(mousePos);

                    if (cardIndex != -1) {
                        Card& card = pile->getCardAt(cardIndex);

                        if (card.isFaceUp()) {
                            startDragging(pile, cardIndex);
                        }
                    }
                }
            }
        }
    }
}

void Solitaire::handleMouseReleased(const sf::Event::MouseButtonReleased& event) {
    if (event.button == sf::Mouse::Button::Left && isDragging) {
        sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.position.x), static_cast<float>(event.position.y));
        stopDragging(mousePos);
    }
}

void Solitaire::handleMouseMoved(const sf::Event::MouseMoved& event) {
    if (isDragging) {
        sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.position.x), static_cast<float>(event.position.y));
        updateDragging(mousePos);
    }
}

void Solitaire::render() {
    window.clear(sf::Color(1, 62, 46));

    if (gameStarted) {
        menuBar.draw(window);
        game.getStock().draw(window);
        game.getWaste().draw(window);

        for (const auto& tableau : game.getTableaus()) {
            tableau.draw(window);
        }

        for (const auto& foundation : game.getFoundations()) {
            foundation.draw(window);
        }

        winScreen.draw(window);

        highlightEffect.draw(window);

        if (isDragging) {
            for (auto& card : draggedCards) {
                card.draw(window);
            }
        }
    }

    startMenu.draw(window);

    if (helpWindow.isOpen()) {
        helpWindow.render();
    }

    window.display();
}

Pile* Solitaire::getPileAt(sf::Vector2f position)
{
    if (game.getWaste().contains(position)) {
        return &const_cast<Waste&>(game.getWaste());
    }

    if (game.getStock().contains(position)) {
        return &const_cast<Stock&>(game.getStock());
    }

    for (int i = 0; i < 7; i++) {
        if (game.getTableaus()[i].contains(position)) {
            return &const_cast<Tableau&>(game.getTableaus()[i]);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (game.getFoundations()[i].contains(position)) {
            return &const_cast<Foundation&>(game.getFoundations()[i]);
        }
    }

    return nullptr;
}

void Solitaire::startDragging(Pile* pile, int cardIndex)
{
    if (!pile || cardIndex < 0) return;

    if (!pile->canTakeCardFrom(cardIndex)) {
        return;
    }

    draggedCards.clear();
    sourcePile = pile;
    startDragIndex = cardIndex;

    startDragMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (Tableau* tableau = dynamic_cast<Tableau*>(pile)) {
        if (tableau->canTakeCardFrom(cardIndex)) {
            draggedCards = tableau->takeCardsFrom(cardIndex);
        }
    }
    else if (Waste* waste = dynamic_cast<Waste*>(pile)) {
        if (waste->canTakeCardFrom(cardIndex)) {
            Card topCard = waste->removeTopCard();
            draggedCards.push_back(topCard);
        }
    }

    if (!draggedCards.empty()) {
        sf::Vector2f firstCardPos = sourcePile->getCardPosition(cardIndex);

        dragOffset = firstCardPos - startDragMousePos;

        for (size_t i = 0; i < draggedCards.size(); i++) {
            sf::Vector2f pos = startDragMousePos + dragOffset;
            pos.y += i * 25.0f;
            draggedCards[i].setPosition(pos);
        }

        isDragging = true;
    }
}

void Solitaire::updateDragging(sf::Vector2f mousePos)
{
    if (draggedCards.empty()) return;

    for (size_t i = 0; i < draggedCards.size(); i++) {
        sf::Vector2f newPos = mousePos + dragOffset;

        newPos.y += i * 25.0f;

        draggedCards[i].setPosition(newPos);
    }
}

void Solitaire::stopDragging(sf::Vector2f mousePos)
{
    if (!isDragging || draggedCards.empty()) return;

    Pile* targetPile = getPileAt(mousePos);

    bool moveSuccessful = false;

    if (targetPile && targetPile != sourcePile) {
        if (Foundation* foundation = dynamic_cast<Foundation*>(targetPile)) {
            if (foundation->canAddCard(draggedCards[0])) {
                foundation->addCard(draggedCards[0]);
                moveSuccessful = true;

                for (size_t i = 1; i < draggedCards.size(); i++) {
                    sourcePile->addCard(draggedCards[i]);
                }
            }
        }
        else {
            if (targetPile->canAddCard(draggedCards[0])) {
                moveSuccessful = true;
            }
        }
    }

    if (moveSuccessful && !dynamic_cast<Foundation*>(targetPile)) {
        for (auto& card : draggedCards) {
            targetPile->addCard(card);
        }
    }
    else if (!moveSuccessful) {
        for (auto& card : draggedCards) {
            sourcePile->addCard(card);
        }
    }

    if (!sourcePile->isEmpty() && !sourcePile->getTopCard().isFaceUp()) {
        sourcePile->revealTopCard();
    }

    startDragIndex = -1;
    draggedCards.clear();
    sourcePile = nullptr;
    isDragging = false;
}
