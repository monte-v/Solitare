#include "Solitaire.h"
#include <iostream>

Solitaire::Solitaire() : window(sf::VideoMode({ 1200, 800 }), "Пасьянс Косынка - SFML 3")
    //sourcePile(nullptr),
    //isDragging(false) 
{

    //backgroundSprite = std::make_shared<sf::Sprite>();

    // SFML 3: VideoMode принимает sf::Vector2u
    //window.setFramerateLimit(60);


    std::cout << "Загрузка текстур карт..." << std::endl;
    if (!Card::loadTextures("", "assets/cards/cards_1/shirt.png")) {
        std::cerr << "Предупреждение: текстуры не загружены" << std::endl;
    }

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

    // Инициализация игры
    //game.newGame();


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
        else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPressed(*keyPressed);
        }
    }
}
void Solitaire::handleMousePressed(const sf::Event::MouseButtonPressed& event) {}
void Solitaire::handleMouseReleased(const sf::Event::MouseButtonReleased& event) {}
void Solitaire::handleMouseMoved(const sf::Event::MouseMoved& event) {}
void Solitaire::handleKeyPressed(const sf::Event::KeyPressed& event) {}

//void Solitaire::handleMousePressed(const sf::Event::MouseButtonPressed& event) {
//    if (event.button == sf::Mouse::Button::Left) {
//        sf::Vector2f lastMousePos = sf::Vector2f(event.position);
//
//        // Находим стопку под курсором
//        Pile* pile = getPileAt(lastMousePos);
//        if (pile && !pile->isEmpty()) {
//            // Находим карту под курсором
//            int cardIndex = pile->getCardIndexAt(lastMousePos);
//            if (cardIndex != -1) {
//                Card& card = pile->getCardAt(cardIndex);
//                if (card.isFaceUp()) {
//                    startDragging(card, *pile);
//                }
//            }
//        }
//    }
//}

//void Solitaire::startDragging(Card& card, Pile& pile) {
//    draggedCards.clear();
//
//    // Если это Tableau, берем все карты от выбранной до верха
//    if (Tableau* tableau = dynamic_cast<Tableau*>(&pile)) {
//        int startIndex = pile.getCardIndexAt(lastMousePos);
//        if (tableau->canTakeCardFrom(startIndex)) {
//            // Здесь логика для взятия группы карт
//            // Пока берем только одну
//            draggedCards.push_back(&card);
//        }
//    }
//    else {
//        // Для других стопок берем только верхнюю карту
//        draggedCards.push_back(&pile.getTopCard());
//    }
//
//    sourcePile = &pile;
//    dragOffset = lastMousePos - card.getPosition();
//    isDragging = true;
//}

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
    std::cout << "Рисую сток..." << std::endl;
    game.getStock().draw(window);

    ////// Рисуем все стопки
    std::cout << "Рисую Tableau..." << std::endl;
    for (const auto& tableau : game.getTableaus()) {
        tableau.draw(window);
    }

    std::cout << "Рисую Foundation..." << std::endl;
    for (const auto& foundation : game.getFoundations()) {
        foundation.draw(window);
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

    //sf::Vector2f pos(50, 50);

    ////// Рисуем сток
    //sf::RectangleShape stockRect(sf::Vector2f(100, 140));
    //stockRect.setPosition({ 50, 50 });
    //stockRect.setFillColor(sf::Color::Blue);
    //stockRect.setOutlineColor(sf::Color::White);
    //stockRect.setOutlineThickness(2);
    //window.draw(stockRect);

    ////// Рисуем Tableau
    //for (int i = 0; i < 7; i++) {
    //    sf::RectangleShape tableauRect(sf::Vector2f(100, 140));
    //    tableauRect.setPosition({ 50.0f + i * 110.0f, 250.0f });
    //    tableauRect.setFillColor(sf::Color(100, 100, 200));
    //    tableauRect.setOutlineColor(sf::Color::White);
    //    tableauRect.setOutlineThickness(2);
    //    window.draw(tableauRect);
    //}

    ////// Рисуем Foundation
    //for (int i = 0; i < 4; i++) {
    //    sf::RectangleShape foundationRect(sf::Vector2f(100, 140));
    //    foundationRect.setPosition({ 300.0f + i * 110.0f, 50.0f });
    //    foundationRect.setFillColor(sf::Color(200, 100, 100));
    //    foundationRect.setOutlineColor(sf::Color::White);
    //    foundationRect.setOutlineThickness(2);
    //    window.draw(foundationRect);
    //}

    window.display();
    //std::cout << "Рендер успешно отработал" << std::endl;
}