#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.h"
#include "MenuBar.h"
#include "StartMenu.h" 
#include "HelpWindow.h"
#include "HighlightEffect.h"
#include "WinScreen.h"
#include "RecordsManager.h"

class Solitaire {
private:
    sf::RenderWindow window;
    Game game;
    StartMenu startMenu;
    MenuBar menuBar;
    HelpWindow helpWindow;
    WinScreen winScreen;
    HighlightEffect highlightEffect;
    RecordsManager recordsManager;

    std::vector<Card> draggedCards;     
    Pile* sourcePile;                   
    sf::Vector2f dragOffset;           
    bool isDragging;                    
    int startDragIndex;
    sf::Vector2f startDragMousePos;

    bool gameStarted;
    bool gameWon;

    void findAndShowHint();
public:
    Solitaire();

    void run();

    void update(sf::Time deltaTime);

private:
    bool initialize();
    bool loadIcon(std::string path);
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

    void startGame();
    void checkAndAddRecord();
};