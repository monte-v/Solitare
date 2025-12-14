#include "Solitaire.h"
#include <iostream>

Solitaire::Solitaire() : window(sf::VideoMode({ 1200, 800 }), "Пасьянс Косынка - SFML 3"),
    sourcePile(nullptr),
    isDragging(false) 
{

    //backgroundSprite = std::make_shared<sf::Sprite>();

    // SFML 3: VideoMode принимает sf::Vector2u
    //window.setFramerateLimit(60);

    if (!initialize()) {
        std::cerr << "Не удалось инициализировать приложение" << std::endl;
        window.close();
    }
}

bool Solitaire::initialize() {
    // Загрузка ресурсов
    if (!loadResources()) {
        return false;
    }

    try {
        game.newGame();
        std::cout << "Инициализация solitare" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool Solitaire::loadResources() {
    std::cout << "Загрузка текстур карт..." << std::endl;
    //if (!Card::loadTextures("", "assets/cards/cards_1/shirt.png")) {
    //    std::cerr << "Предупреждение: текстуры не загружены" << std::endl;
    //}

    if (!Card::loadBackTexture("assets/cards/cards_1/shirt.png")) {
        std::cerr << "Предупреждение: рубашка не загружена" << std::endl;
        // Можно продолжить, но карты будут без текстур
    }
    if (!Pile::loadEmptyPileTexture("assets/cards/Pile.jpg")) {
        std::cerr << "Предупреждение: текстура пустой стопки не загружена" << std::endl;
    }


    // SFML 3: loadFromFile возвращает optional или bool
    //if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    //    std::cerr << "Не удалось загрузить шрифт" << std::endl;
    //    return false;
    //}

    //backgroundTexture = std::make_shared<sf::Texture>();

    //if (!backgroundTexture->loadFromFile("assets/background.jpg")) {
    //    std::cerr << "Не удалось загрузить фон" << std::endl;
    //    // Можно продолжить без фона
    //}
    //else {
    //    backgroundSprite->setTexture(*backgroundTexture);
    //    backgroundSprite->setScale(sf::Vector2f(
    //        static_cast<float>(window.getSize().x) / backgroundTexture->getSize().x,
    //        static_cast<float>(window.getSize().y) / backgroundTexture->getSize().y
    //    ));
    //}

    //backgroundTexture = std::make_shared<sf::Texture>();

    //if (!backgroundTexture->loadFromFile("assets/background.jpg")) {
    //    std::cerr << "Не удалось загрузить фон" << std::endl;
    //    // Можно продолжить без фона
        return true; // Продолжаем без фона
    //}
    //else {
    //    // SFML 3: создаем Sprite с текстурой сразу
    //    backgroundSprite = std::make_shared<sf::Sprite>(*backgroundTexture);

    //    // Масштабируем
    //    backgroundSprite->setScale(sf::Vector2f(
    //        static_cast<float>(window.getSize().x) / backgroundTexture->getSize().x,
    //        static_cast<float>(window.getSize().y) / backgroundTexture->getSize().y
    //    ));
    //}


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
    //if (game.isGameWon()) {
    //    // Игра выиграна - можно добавить анимацию или эффекты
    //    return;
    //}
}

void Solitaire::processEvents() {
    // SFML 3: новый API событий
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (!event) continue;

        // Проверяем тип события через if вместо switch
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
        //else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        //    handleKeyPressed(*keyPressed);
        //}
    }
}
void Solitaire::handleMousePressed(const sf::Event::MouseButtonPressed& event) {
    if (event.button == sf::Mouse::Button::Left && !isDragging) {
        sf::Vector2f mousePos = sf::Vector2f(event.position.x, event.position.y);
        std::cout << "Клик: " << mousePos.x << ", " << mousePos.y  << std::endl;

        // Находим стопку под курсором
        Pile* pile = getPileAt(mousePos);

        if (pile) {
            std::cout << "Найдена стопка" << std::endl;
            std::cout << "Стопка пустая? : " << pile->isEmpty() << std::endl;

            if (!pile->isEmpty()) {
                // Находим карту под курсором
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
//void Solitaire::handleKeyPressed(const sf::Event::KeyPressed& event) {}

void Solitaire::render() {
    //std::cout << "Начало рендоринга" << std::endl;
    window.clear(sf::Color(0, 100, 0));  // Зеленый фон
    //// Рисуем фон
    //if (backgroundTexture) {
    //    window.draw(*backgroundSprite);
    //}
    //if (backgroundSprite) {
    //    window.draw(*backgroundSprite);
    //}
    // Сток
    game.getStock().draw(window);

    ////// Рисуем все стопки
    for (const auto& tableau : game.getTableaus()) {
        tableau.draw(window);
    }

    for (const auto& foundation : game.getFoundations()) {
        foundation.draw(window);
    }

    if (isDragging) {
        for (auto& card : draggedCards) {
            card.draw(window);
        }
    }

    // Рисуем перетаскиваемые карты поверх всего
    //if (isDragging) {
    //    for (auto* card : draggedCards) {
    //        card->draw(window);
    //    }
    //}

    // Рисуем UI
    //drawScore();
    //if (game.isGameWon()) {
    //    drawGameWonScreen();
    //}

    window.display();
}

Pile* Solitaire::getPileAt(sf::Vector2f position)
{
    if (game.getStock().contains(position)) {
        return &const_cast<Stock&>(game.getStock());
    }

    // Проверяем Tableau
    for (int i = 0; i < 7; i++) {
        if (game.getTableaus()[i].contains(position)) {
            return &const_cast<Tableau&>(game.getTableaus()[i]);
        }
    }

    // Проверяем Foundation
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

    draggedCards.clear();
    sourcePile = pile;

    // Берем все карты от выбранной до верха (для Tableau)
    if (Tableau* tableau = dynamic_cast<Tableau*>(pile)) {
        if (tableau->canTakeCardFrom(cardIndex)) {
            for (int i = cardIndex; i < pile->getCardCount(); i++) {
                draggedCards.push_back(pile->getCardAt(i));
            }
        }
    }
    else {
        // Для других стопок берем только верхнюю
        draggedCards.push_back(pile->getTopCard());
    }

    // Запоминаем смещение от курсора
    if (!draggedCards.empty()) {
        dragOffset = sf::Vector2f(0, 0) - draggedCards[0].getPosition();
        isDragging = true;
    }
}

void Solitaire::updateDragging(sf::Vector2f mousePos)
{
    if (draggedCards.empty()) return;

    // Обновляем позицию всех перетаскиваемых карт
    for (size_t i = 0; i < draggedCards.size(); i++) {
        sf::Vector2f newPos = mousePos + dragOffset;
        newPos.y += i * 25.0f; // Немного смещаем для красоты
        draggedCards[i].setPosition(newPos);
    }
}

void Solitaire::stopDragging(sf::Vector2f mousePos)
{
    if (!isDragging || draggedCards.empty()) return;

    // Находим целевую стопку
    Pile* targetPile = getPileAt(mousePos);

    // Проверяем, можно ли положить карту
    bool moveSuccessful = false;
    if (targetPile && targetPile != sourcePile) {
        // Проверяем первую карту из перетаскиваемых
        if (targetPile->canAddCard(draggedCards[0])) {
            // Если из Tableau берем несколько карт
            if (Tableau* sourceTableau = dynamic_cast<Tableau*>(sourcePile)) {
                if (Tableau* targetTableau = dynamic_cast<Tableau*>(targetPile)) {
                    // Для Tableau в Tableau проверяем возможность
                    if (targetTableau->canAddCard(draggedCards[0])) {
                        moveSuccessful = true;
                    }
                }
                else if (Foundation* targetFoundation = dynamic_cast<Foundation*>(targetPile)) {
                    // В Foundation можно только по одной
                    if (draggedCards.size() == 1 && targetFoundation->canAddCard(draggedCards[0])) {
                        moveSuccessful = true;
                    }
                }
            }
            else {
                // Из других стопок - только по одной карте
                if (draggedCards.size() == 1 && targetPile->canAddCard(draggedCards[0])) {
                    moveSuccessful = true;
                }
            }
        }
    }

    // Выполняем или отменяем перемещение
    if (moveSuccessful) {
        // Удаляем карты из исходной стопки
        if (Tableau* sourceTableau = dynamic_cast<Tableau*>(sourcePile)) {
            // Для Tableau удаляем все перетаскиваемые карты
            int startIndex = sourcePile->getCardIndexAt(draggedCards[0].getPosition());
            if (startIndex != -1) {
                // Удаляем карты из sourcePile
                // (нужно реализовать метод takeCardsFrom в Pile)
                // Пока просто удаляем верхнюю
                sourcePile->removeTopCard();
            }
        }
        else {
            sourcePile->removeTopCard();
        }

        // Добавляем карты в целевую стопку
        for (auto& card : draggedCards) {
            targetPile->addCard(card);
        }

        // Открываем новую верхнюю карту в исходной стопке
        if (!sourcePile->isEmpty() && !sourcePile->getTopCard().isFaceUp()) {
            sourcePile->revealTopCard();
        }
    }

    // Сбрасываем состояние перетаскивания
    draggedCards.clear();
    sourcePile = nullptr;
    isDragging = false;
}
