#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.h"
#include "MenuBar.h"

class Solitaire {
private:
    sf::RenderWindow window;
    Game game;
    MenuBar menuBar;

    std::vector<Card> draggedCards;     // Карты, которые перетаскиваем
    Pile* sourcePile;                   // Откуда взяли карты
    sf::Vector2f dragOffset;            // Смещение от курсора
    bool isDragging;                    // Флаг перетаскивания
    int startDragIndex;
    sf::Vector2f startDragMousePos;
public:
    Solitaire();

    void run();

    void update(sf::Time deltaTime);

private:
    bool initialize();
    bool loadResources();

    void processEvents();
    void handleMousePressed(const sf::Event::MouseButtonPressed& event);
    void handleMouseReleased(const sf::Event::MouseButtonReleased& event);
    void handleMouseMoved(const sf::Event::MouseMoved& event);

    void render();

    Pile* getPileAt(sf::Vector2f position);
    void startDragging(Pile* pile, int cardIndex);
    void updateDragging(sf::Vector2f mousePos);
    void stopDragging(sf::Vector2f mousePos);
};