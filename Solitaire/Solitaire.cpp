#include "Solitaire.h"
#include "HighlightEffect.h"
#include <iostream>

Solitaire::Solitaire() : window(sf::VideoMode({ 1000, 800 }), "Solitaire"), 
    sourcePile(nullptr), isDragging(false), startDragMousePos(0, 0), gameStarted(false) {
    if (!initialize()) {
        std::cerr << "Не удалось инициализировать приложение" << std::endl;
        window.close();
    }
}

bool Solitaire::initialize() {
    if (!loadResources()) {
        return false;
    }

    menuBar.setSize(sf::Vector2f(1000, 35));
    menuBar.setPosition(sf::Vector2f(0, 0));

    menuBar.setOnNewGameCallback([this]() {
        std::cout << "Запуск новой игры через меню..." << std::endl;
        try {
            game.newGame();

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

    if (!startMenu.initialize("assets/fonts/arial.ttf")) {
        std::cerr << "Не удалось инициализировать стартовое меню" << std::endl;
        // Можно продолжить, но меню будет без текста
    }

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
                    // Используем позицию верхней карты и добавляем отступ для следующей
                    targetPos = tableau.getTopCard().getPosition();
                    targetPos.y += 25.0f; // Используем фиксированный отступ (как в Tableau конструкторе)
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

    if (!menuBar.loadFont("assets/fonts/arial.ttf")) {
        std::cerr << "Шрифт для меню не загружен" << std::endl;
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
        //update(deltaTime);

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
}

void Solitaire::processEvents() {
    /*while (std::optional<sf::Event> event = window.pollEvent()) {
        if (!event) continue;

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            handleMousePressed(*mousePressed);
        }
        else if (auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            handleMouseReleased(*mouseReleased);
        }
        else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            handleMouseMoved(*mouseMoved);
        }
    }*/

    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (!event) continue;

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            //sf::Vector2f mousePos = window.mapPixelToCoords(
            //    sf::Vector2i(mousePressed->position.x, mousePressed->position.y)
            //);

            //// Проверяем, кликнули ли в MenuBar
            //if (mousePos.y <= 35) {
            //    menuBar.handleClick(mousePos);
            //}
            //else {
            //    handleMousePressed(*mousePressed);
            //}

            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(mousePressed->position.x, mousePressed->position.y)
            );

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
        sf::Vector2f mousePos = sf::Vector2f(event.position.x, event.position.y);
        stopDragging(mousePos);
    }
}

void Solitaire::handleMouseMoved(const sf::Event::MouseMoved& event) {
    if (isDragging) {
        sf::Vector2f mousePos = sf::Vector2f(event.position.x, event.position.y);
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
    //if (!pile || cardIndex < 0) return;

    //std::cout << "=== startDragging ===" << std::endl;

    //draggedCards.clear();
    //sourcePile = pile;
    //startDragIndex = cardIndex;  // Если используешь это поле

    //// Получаем текущую позицию мыши
    //startDragMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    //std::cout << "Начальная позиция мыши: "
    //<< startDragMousePos.x << ", " << startDragMousePos.y << std::endl;

    //if (Tableau* tableau = dynamic_cast<Tableau*>(pile)) {
    //    if (tableau->canTakeCardFrom(cardIndex)) {
    //        std::cout << "Беру " << (pile->getCardCount() - cardIndex) << " карт" << std::endl;
    //        for (int i = cardIndex; i < pile->getCardCount(); i++) {
    //            draggedCards.push_back(pile->getCardAt(i));
    //        }
    //    }
    //}
    //else {
    //    draggedCards.push_back(pile->getTopCard());
    //}

    //if (!draggedCards.empty()) {
    //    // ВАЖНО: позиция первой карты ДО начала перетаскивания
    //    sf::Vector2f firstCardPos = draggedCards[0].getPosition();
    //    std::cout << "Позиция первой карты: "
    //        << firstCardPos.x << ", " << firstCardPos.y << std::endl;

    //    // Правильное вычисление смещения:
    //    // Насколько карта смещена относительно курсора
    //    dragOffset = firstCardPos - startDragMousePos;

    //    std::cout << "dragOffset вычислен: "
    //        << dragOffset.x << ", " << dragOffset.y << std::endl;

    //    isDragging = true;
    //}

    //std::cout << "=== конец startDragging ===" << std::endl;


    if (!pile || cardIndex < 0) return;

    std::cout << "=== startDragging ===" << std::endl;

    draggedCards.clear();
    sourcePile = pile;
    startDragIndex = cardIndex;

    // Получаем текущую позицию мыши
    startDragMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (Tableau* tableau = dynamic_cast<Tableau*>(pile)) {
        if (tableau->canTakeCardFrom(cardIndex)) {
            // ВАЖНО: БЕРЕМ КАРТЫ ИЗ СТОПКИ (удаляем их)
            draggedCards = tableau->takeCardsFrom(cardIndex);
            std::cout << "Взято карт: " << draggedCards.size() << std::endl;
        }
    }
    else {
        // Для других стопок берем только верхнюю
        Card topCard = pile->removeTopCard();  // Удаляем из стопки
        draggedCards.push_back(topCard);
    }

    if (!draggedCards.empty()) {
        // Позиция первой карты ДО начала перетаскивания
        sf::Vector2f firstCardPos = sourcePile->getCardPosition(cardIndex);

        // Вычисляем смещение
        dragOffset = firstCardPos - startDragMousePos;

        // Устанавливаем позицию для перетаскивания
        for (size_t i = 0; i < draggedCards.size(); i++) {
            sf::Vector2f pos = startDragMousePos + dragOffset;
            pos.y += i * 25.0f;
            draggedCards[i].setPosition(pos);
        }

        isDragging = true;
    }

    std::cout << "=== конец startDragging ===" << std::endl;

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
    //if (!isDragging || draggedCards.empty()) return;

    //Pile* targetPile = getPileAt(mousePos);

    //bool moveSuccessful = false;
    //if (targetPile && targetPile != sourcePile) {
    //    // Проверяем первую карту из перетаскиваемых
    //    if (targetPile->canAddCard(draggedCards[0])) {
    //        // Если из Tableau берем несколько карт
    //        if (Tableau* sourceTableau = dynamic_cast<Tableau*>(sourcePile)) {
    //            if (Tableau* targetTableau = dynamic_cast<Tableau*>(targetPile)) {
    //                // Для Tableau в Tableau проверяем возможность
    //                if (targetTableau->canAddCard(draggedCards[0])) {
    //                    moveSuccessful = true;
    //                }
    //            }
    //            else if (Foundation* targetFoundation = dynamic_cast<Foundation*>(targetPile)) {
    //                // В Foundation можно только по одной
    //                if (draggedCards.size() == 1 && targetFoundation->canAddCard(draggedCards[0])) {
    //                    moveSuccessful = true;
    //                }
    //            }
    //        }
    //        else {
    //            // Из других стопок - только по одной карте
    //            if (draggedCards.size() == 1 && targetPile->canAddCard(draggedCards[0])) {
    //                moveSuccessful = true;
    //            }
    //        }
    //    }
    //}

    //// Выполняем или отменяем перемещение
    //if (moveSuccessful) {
    //    // Удаляем карты из исходной стопки
    //    if (Tableau* sourceTableau = dynamic_cast<Tableau*>(sourcePile)) {
    //        // Для Tableau удаляем все перетаскиваемые карты
    //        //int startIndex = sourcePile->getCardIndexAt(draggedCards[0].getPosition());
    //        //if (startIndex != -1) {
    //        //    // Удаляем карты из sourcePile
    //        //    // (нужно реализовать метод takeCardsFrom в Pile)
    //        //    // Пока просто удаляем верхнюю
    //        //    sourcePile->removeTopCard();
    //        //}
    //        int startIndex = startDragIndex;
    //        if (startIndex != -1) {
    //            // Удаляем ВСЕ карты от startIndex до конца
    //            sourceTableau->takeCardsFrom(startIndex);
    //        }
    //    }
    //    else {
    //        sourcePile->removeTopCard();
    //    }

    //    // Добавляем карты в целевую стопку
    //    for (auto& card : draggedCards) {
    //        targetPile->addCard(card);
    //    }

    //    // Открываем новую верхнюю карту в исходной стопке
    //    if (!sourcePile->isEmpty() && !sourcePile->getTopCard().isFaceUp()) {
    //        sourcePile->revealTopCard();
    //    }
    //}

    //// Сбрасываем состояние перетаскивания
    //startDragIndex = -1;
    //draggedCards.clear();
    //sourcePile = nullptr;
    //isDragging = false;


    if (!isDragging || draggedCards.empty()) return;

    Pile* targetPile = getPileAt(mousePos);

    bool moveSuccessful = false;

    if (targetPile && targetPile != sourcePile) {
        if (targetPile->canAddCard(draggedCards[0])) {
            // Проверяем правила для разных типов стопок...
            moveSuccessful = true;
        }
    }

    if (moveSuccessful) {
        // Карты УЖЕ удалены из sourcePile в startDragging()
        // Просто добавляем их в целевую стопку
        for (auto& card : draggedCards) {
            targetPile->addCard(card);
        }

        // Открываем новую верхнюю карту в исходной стопке
        if (!sourcePile->isEmpty() && !sourcePile->getTopCard().isFaceUp()) {
            sourcePile->revealTopCard();
        }
    }
    else {
        // ВОЗВРАЩАЕМ карты обратно в исходную стопку
        for (auto& card : draggedCards) {
            sourcePile->addCard(card);
        }
    }

    // Сбрасываем состояние
    startDragIndex = -1;
    draggedCards.clear();
    sourcePile = nullptr;
    isDragging = false;
}
