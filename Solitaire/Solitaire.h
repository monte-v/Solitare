#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.h"

class Solitaire {
private:
    // SFML 3: Окно создается через unique_ptr или напрямую
    sf::RenderWindow window;
    Game game;

    // Ресурсы
    //sf::Font font;
    //std::shared_ptr<sf::Texture> backgroundTexture;
    //std::shared_ptr<sf::Sprite> backgroundSprite;

    //// Перетаскивание
    //std::vector<Card*> draggedCards;
    Pile* sourcePile;
    //sf::Vector2f dragOffset;

    //// UI состояние
    bool isDragging;
    //sf::Vector2f lastMousePos;

public:
    Solitaire();

    // Главный цикл
    void run();

    void update(sf::Time deltaTime);

private:
    // Инициализация
    bool initialize();
    bool loadResources();

    // Обработка событий (SFML 3: Event теперь в sf::window)
    void processEvents();
    void handleMousePressed(const sf::Event::MouseButtonPressed& event);
    void handleMouseReleased(const sf::Event::MouseButtonReleased& event);
    void handleMouseMoved(const sf::Event::MouseMoved& event);
    void handleKeyPressed(const sf::Event::KeyPressed& event);

    // Обновление и отрисовка
    //void update(sf::Time deltaTime);
    void render();

    // Логика игры
    //Pile* getPileAt(sf::Vector2f position);
    //void startDragging(Card& card, Pile& pile);
    //void stopDragging();

    // Вспомогательные методы
    //void drawUI();
    //void drawScore();
    //void drawGameWonScreen();
};