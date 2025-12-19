#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Button.h"
#include "RecordsTable.h"

class StartMenu {
private:
    bool visible;
    sf::Font font;
    std::optional<sf::Texture> backgroundTexture; 
    std::optional<sf::Sprite> backgroundSprite;

    Button startButton;
    Button recordsButton;
    Button helpButton;
    Button exitButton;

    std::function<void()> onStartCallback;
    std::function<void()> onRecordsCallback;  
    std::function<void()> onHelpCallback;
    std::function<void()> onExitCallback;

    std::unique_ptr<sf::Text> title;

    RecordsTable recordsTable;  
    bool showRecords;

public:
    StartMenu();

    bool initialize(const std::string& fontPath, const std::string& texturePath);

    void setOnStart(const std::function<void()>& callback);
    void setOnRecords(const std::function<void()>& callback);
    void setOnHelp(const std::function<void()>& callback);
    void setOnExit(const std::function<void()>& callback);

    void setRecordsManager(RecordsManager* manager);

    void show() { visible = true; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void showRecordsTable() { showRecords = true; recordsTable.show(); }
    void hideRecordsTable() { showRecords = false; } 
    bool isShowingRecords() const { return showRecords; }

    void updateRecordsTable();

    void handleClick(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window) const;

private:
    void createUX();
};