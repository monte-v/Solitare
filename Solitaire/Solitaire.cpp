#include "Solitaire.h"
#include <iostream>

Solitaire::Solitaire() : window(sf::VideoMode({ 1000, 800 }), "Solitaire", sf::Style::Titlebar | sf::Style::Close),
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

    menuBar.setOnNewGameCallback([this]() {
        std::cout << "Запуск новой игры через меню..." << std::endl;
        try {
            game.newGame();
            gameWon = false; // Сбрасываем флаг победы
            winScreen.hide();
            isDragging = false;
            draggedCards.clear();
            sourcePile = nullptr;
            startDragIndex = -1;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    });
    menuBar.setOnHelpCallback([this]() {
        std::cout << "Открыть помощь..." << std::endl;
        helpWindow.open();
    });
    menuBar.setOnHintCallback([this]() {
        if (gameStarted) {
            findAndShowHint();
        }
    });

    startMenu.setOnStart([this]() {
        std::cout << "Начало игры из стартового меню..." << std::endl;
        startGame();
        });

    startMenu.setOnHelp([this]() {
        std::cout << "Показать помощь из стартового меню..." << std::endl;
        helpWindow.open();
        });

    startMenu.setOnExit([this]() {
        std::cout << "Выход из игры..." << std::endl;
        window.close();
        });

    // Сразу показываем стартовое меню
    startMenu.show();


    //try {
    //    game.newGame();
    //    std::cout << "Инициализация solitare" << std::endl;
    //}
    //catch (const std::exception& e) {
    //    std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
    //    return false;
    //}

    return true;
}

void Solitaire::findAndShowHint() {
    highlightEffect.clear();

    // Проверяем верхние карты во всех стопках
    std::vector<Pile*> allPiles;

    // Добавляем Waste если есть карты
    if (!game.getWaste().isEmpty()) {
        allPiles.push_back(&const_cast<Waste&>(game.getWaste()));
    }

    // Добавляем Tableaus
    for (int i = 0; i < 7; i++) {
        if (!game.getTableaus()[i].isEmpty()) {
            allPiles.push_back(&const_cast<Tableau&>(game.getTableaus()[i]));
        }
    }

    // Для каждой стопки проверяем, куда можно положить ее верхнюю карту
    for (Pile* sourcePile : allPiles) {
        if (sourcePile->isEmpty()) continue;

        const Card& sourceCard = sourcePile->getTopCard();

        // Проверяем Foundations
        for (int i = 0; i < 4; i++) {
            Foundation& foundation = const_cast<Foundation&>(game.getFoundations()[i]);
            if (foundation.canAddCard(sourceCard)) {
                // Подсвечиваем карту-источник и фонд, куда ее можно положить
                sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                highlightEffect.addHighlight(sourcePos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                sf::Vector2f targetPos = foundation.getPosition();
                highlightEffect.addHighlight(targetPos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                return; // Нашли первый возможный ход
            }
        }

        // Проверяем Tableaus
        for (int i = 0; i < 7; i++) {
            Tableau& tableau = const_cast<Tableau&>(game.getTableaus()[i]);
            if (&tableau != sourcePile && tableau.canAddCard(sourceCard)) {
                // Подсвечиваем карту-источник и tableau, куда ее можно положить
                sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                highlightEffect.addHighlight(sourcePos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                sf::Vector2f targetPos;
                if (tableau.isEmpty()) {
                    targetPos = tableau.getPosition();
                }
                else {
                    // Получаем позицию для следующей карты в Tableau
                    // Нужна позиция для карты с индексом равным текущему количеству карт
                    targetPos = tableau.getCardPosition(static_cast<int>(tableau.getCardCount()));
                }
                highlightEffect.addHighlight(targetPos,
                    sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                return; // Нашли первый возможный ход
            }
        }
    }

    // Проверяем пустые Tableaus - можно положить короля
    for (int i = 0; i < 7; i++) {
        Tableau& tableau = const_cast<Tableau&>(game.getTableaus()[i]);
        if (tableau.isEmpty()) {
            // Ищем короля во всех стопках
            for (Pile* sourcePile : allPiles) {
                if (sourcePile->isEmpty()) continue;

                const Card& sourceCard = sourcePile->getTopCard();
                if (sourceCard.getRank() == Rank::King) {
                    // Подсвечиваем короля и пустой tableau
                    sf::Vector2f sourcePos = sourcePile->getTopCard().getPosition();
                    highlightEffect.addHighlight(sourcePos,
                        sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                    sf::Vector2f targetPos = tableau.getPosition();
                    highlightEffect.addHighlight(targetPos,
                        sf::Vector2f(Card::WIDTH, Card::HEIGHT), 3.0f);

                    return; // Нашли возможный ход
                }
            }
        }
    }

    // Если не нашли ходов - можно попробовать взять из Stock
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
        std::cout << "Игра начата!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
    }
}

bool Solitaire::loadResources() {
    std::cout << "Загрузка текстур карт..." << std::endl;

    if (!startMenu.initialize("assets/fonts/arial.ttf")) {
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
    if (!Pile::loadEmptyPileTexture("assets/cards/Pile_1/Pile.jpg")) {
        std::cerr << "Предупреждение: текстура пустой стопки не загружена" << std::endl;
    }
    if (!Foundation::loadTextures("assets/cards/Pile_1/")) {
        std::cerr << "Текстура Foundation не загружена" << std::endl;
        // Можно использовать цветной прямоугольник как запасной вариант
    }

    return true; 
}

void Solitaire::run() {
    sf::Clock clock;


    std::cout << "Игра запущена" << std::endl;

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
    //if (game.isGameWon()) {
    //    // Игра выиграна - можно добавить анимацию или эффекты
    //    return;
    //}
    if (gameStarted && !gameWon && !winScreen.isVisible()) {
        bool allFoundationsComplete = true;
        for (int i = 0; i < 4; i++) {
            if (!game.getFoundations()[i].isComplete()) {
                allFoundationsComplete = false;
                break;
            }
        }

        if (allFoundationsComplete) {
            gameWon = true;
            // Показываем экран победы с временем
            std::string gameTime = game.getFormattedTime();
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
        else if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(mousePressed->position.x, mousePressed->position.y)
            );

            if (winScreen.isVisible()) {
                if (winScreen.handleClick(mousePos.x, mousePos.y)) {
                    // Начинаем новую игру при клике
                    game.newGame();
                    gameWon = false;
                    return;
                }
            }

            // Если видно стартовое меню
            if (startMenu.isVisible()) {
                // Клик в стартовом меню
                startMenu.handleClick(mousePos);
            }
            // Игра активна
            else {
                // Клик в MenuBar
                if (mousePos.y <= 35) {
                    menuBar.handleClick(mousePos);
                }
                // Клик в игровом поле
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
        sf::Vector2f mousePos /*= sf::Vector2f(event.position.x, event.position.y);*/ = window.mapPixelToCoords(sf::Vector2i(event.position.x, event.position.y));
        //std::cout << "Клик: " << mousePos.x << ", " << mousePos.y  << std::endl;
        std::cout << "Клик (окно): " << event.position.x << ", " << event.position.y << std::endl;
        std::cout << "Клик (мир): " << mousePos.x << ", " << mousePos.y << std::endl;
        std::cout << "Проверка стопок под курсором:" << std::endl;

        // Проверяем Stock
        std::cout << "  Stock содержит? " << game.getStock().contains(mousePos)
            << ", пустой? " << game.getStock().isEmpty() << std::endl;

        // Проверяем Tableau
        for (int i = 0; i < 7; i++) {
            std::cout << "  Tableau[" << i << "] содержит? "
                << game.getTableaus()[i].contains(mousePos)
                << ", карт: " << game.getTableaus()[i].getCardCount()
                << ", пустой? " << game.getTableaus()[i].isEmpty() << std::endl;
        }

        // Проверяем Foundation
        for (int i = 0; i < 4; i++) {
            std::cout << "  Foundation[" << i << "] содержит? "
                << game.getFoundations()[i].contains(mousePos)
                << ", пустой? " << game.getFoundations()[i].isEmpty() << std::endl;
        }

        if (mousePos.y > 35) {
            Pile* pile = getPileAt(mousePos);

            if (pile) {
                std::cout << "Найдена стопка" << std::endl;
                std::cout << "Стопка пустая? : " << pile->isEmpty() << std::endl;

                if (dynamic_cast<Stock*>(pile)) {
                    // ВАРИАНТ 1: Замена карт в Waste
                    game.drawFromStock();

                    // ВАРИАНТ 2: Если хотите разделить логику
                    // if (game.getStock().isEmpty()) {
                    //     // Если Stock пуст - возвращаем карты из Waste
                    //     game.returnWasteToStock();
                    // } else {
                    //     // Иначе - берем 1 карту в Waste
                    //     game.drawOneCardToWaste();
                    // }

                    return;
                }

                if (!pile->isEmpty()) {
                    int cardIndex = pile->getCardIndexAt(mousePos);
                    std::cout << "Индекс карты: " << cardIndex << std::endl;

                    if (cardIndex != -1) {
                        Card& card = pile->getCardAt(cardIndex);
                        std::cout << "Карта лицом вверх? : " << card.isFaceUp() << std::endl;

                        if (card.isFaceUp()) {
                            std::cout << "Начало перетаскивания..." << std::endl;
                            startDragging(pile, cardIndex);
                        }
                    }
                }
            }
            else {
                std::cout << "Стопка не найдена" << std::endl;
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
    window.clear(sf::Color(0, 100, 0));

    if (gameStarted) {
        // Рисуем игровое поле
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

    // Рисуем стартовое меню (если видимо) поверх всего
    startMenu.draw(window);

    // Рисуем окно помощи (если видимо)
    if (helpWindow.isOpen()) {
        helpWindow.render();
    }

    window.display();

    //menuBar.draw(window);

    //game.getStock().draw(window);
    //game.getWaste().draw(window);

    //for (const auto& tableau : game.getTableaus()) {
    //    tableau.draw(window);
    //}

    //for (const auto& foundation : game.getFoundations()) {
    //    foundation.draw(window);
    //}

    //if (isDragging) {
    //    for (auto& card : draggedCards) {
    //        card.draw(window);
    //    }
    //}

    //window.display();
    //if (helpWindow.isOpen()) {
    //    helpWindow.render();
    //}
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

    //Pile* foundPile = nullptr;
    //float highestZ = -1; // Ищем самую верхнюю карту

    //// Проверяем Tableau
    //for (int i = 0; i < 7; i++) {
    //    if (game.getTableaus()[i].contains(position)) {
    //        // Если в этой стопке есть карты, берем индекс верхней
    //        if (!game.getTableaus()[i].isEmpty()) {
    //            float z = game.getTableaus()[i].getCardCount(); // Чем больше карт, тем "выше"
    //            if (z > highestZ) {
    //                highestZ = z;
    //                foundPile = &const_cast<Tableau&>(game.getTableaus()[i]);
    //            }
    //        }
    //    }
    //}

    //// Проверяем Foundation
    //for (int i = 0; i < 4; i++) {
    //    if (game.getFoundations()[i].contains(position)) {
    //        float z = game.getFoundations()[i].getCardCount();
    //        if (z > highestZ) {
    //            highestZ = z;
    //            foundPile = &const_cast<Foundation&>(game.getFoundations()[i]);
    //        }
    //    }
    //}

    //// Проверяем Stock
    //if (game.getStock().contains(position)) {
    //    float z = game.getStock().getCardCount();
    //    if (z > highestZ) {
    //        foundPile = &const_cast<Stock&>(game.getStock());
    //    }
    //}

    //return foundPile;
    return nullptr;
}

void Solitaire::startDragging(Pile* pile, int cardIndex)
{
    if (!pile || cardIndex < 0) return;

    //std::cout << "=== startDragging ===" << std::endl;

    // Проверяем, можно ли вообще брать карты из этой стопки
    if (!pile->canTakeCardFrom(cardIndex)) {
        //std::cout << "Нельзя брать карты из этой стопки!" << std::endl;
        return;
    }

    draggedCards.clear();
    sourcePile = pile;
    startDragIndex = cardIndex;

    startDragMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (Tableau* tableau = dynamic_cast<Tableau*>(pile)) {
        if (tableau->canTakeCardFrom(cardIndex)) {
            draggedCards = tableau->takeCardsFrom(cardIndex);
            //std::cout << "Взято карт: " << draggedCards.size() << std::endl;
        }
    }
    else if (Waste* waste = dynamic_cast<Waste*>(pile)) {
        // Для Waste можно брать только верхнюю карту
        if (waste->canTakeCardFrom(cardIndex)) {
            Card topCard = waste->removeTopCard();
            draggedCards.push_back(topCard);
        }
    }
    else if (Foundation* foundation = dynamic_cast<Foundation*>(pile)) {
        // Для Foundation проверяем, можно ли брать
        if (foundation->canTakeCardFrom(cardIndex)) {
            Card topCard = foundation->removeTopCard();
            draggedCards.push_back(topCard);
        }
        else {
            // Если нельзя брать из Foundation, очищаем и выходим
            sourcePile = nullptr;
            return;
        }
    }
    else {
        // Для других стопок (Stock и т.д.)
        if (pile->canTakeCardFrom(cardIndex)) {
            Card topCard = pile->removeTopCard();
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
    //if (draggedCards.empty()) return;

    //for (size_t i = 0; i < draggedCards.size(); i++) {
    //    sf::Vector2f newPos = mousePos + dragOffset;
    //    newPos.y += i * 25.0f; 
    //    draggedCards[i].setPosition(newPos);
    //}



    if (draggedCards.empty()) return;

    // Обновляем позицию всех перетаскиваемых карт
    for (size_t i = 0; i < draggedCards.size(); i++) {
        // Новая позиция = позиция_мыши + смещение_карты_относительно_мыши
        sf::Vector2f newPos = mousePos + dragOffset;

        // Добавляем каскадный эффект для остальных карт
        newPos.y += i * 25.0f;

        draggedCards[i].setPosition(newPos);

        //// Отладка для первой карты
        //if (i == 0) {
        //    std::cout << "updateDragging: мышь=" << mousePos.x << "," << mousePos.y
        //        << ", newPos=" << newPos.x << "," << newPos.y
        //        << ", dragOffset=" << dragOffset.x << "," << dragOffset.y << std::endl;
        //}
    }
}

void Solitaire::stopDragging(sf::Vector2f mousePos)
{
    if (!isDragging || draggedCards.empty()) return;

    Pile* targetPile = getPileAt(mousePos);

    bool moveSuccessful = false;

    if (targetPile && targetPile != sourcePile) {
        // Если целевая стопка - Foundation, проверяем можно ли положить только ВЕРХНЮЮ карту
        if (Foundation* foundation = dynamic_cast<Foundation*>(targetPile)) {
            // В Foundation можно положить только ОДНУ карту (верхнюю из перетаскиваемых)
            if (foundation->canAddCard(draggedCards[0])) {
                // Кладем только первую (верхнюю) карту
                foundation->addCard(draggedCards[0]);
                moveSuccessful = true;

                // Остальные карты возвращаем обратно в sourcePile
                for (size_t i = 1; i < draggedCards.size(); i++) {
                    sourcePile->addCard(draggedCards[i]);
                }
            }
        }
        else {
            // Для Tableau и других стопок проверяем можно ли положить всю группу
            if (targetPile->canAddCard(draggedCards[0])) {
                moveSuccessful = true;
            }
        }
    }

    if (moveSuccessful && !dynamic_cast<Foundation*>(targetPile)) {
        // Если перемещение успешно и НЕ в Foundation - добавляем ВСЕ карты
        for (auto& card : draggedCards) {
            targetPile->addCard(card);
        }
    }
    else if (!moveSuccessful) {
        // Если перемещение не удалось - возвращаем все карты обратно
        for (auto& card : draggedCards) {
            sourcePile->addCard(card);
        }
    }

    // Если перемещение в Foundation было успешным, мы уже обработали карты выше
    // Не нужно их обрабатывать здесь снова

    if (!sourcePile->isEmpty() && !sourcePile->getTopCard().isFaceUp()) {
        sourcePile->revealTopCard();
    }

    startDragIndex = -1;
    draggedCards.clear();
    sourcePile = nullptr;
    isDragging = false;
}
