#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "RecordsManager.h"

class RecordsTable {
private:
    sf::RectangleShape background;
    sf::RectangleShape tableBackground;

    std::optional<sf::Text> titleText;
    std::optional<sf::Text> timeText;

    std::vector<std::optional<sf::Text>> timeTexts;

    bool visible;
    RecordsManager* recordsManager;

public:
    RecordsTable();

    void initialize(sf::Font& font);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);

    void show() { visible = true; update(); }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }

    void setRecordsManager(RecordsManager* manager) {
        recordsManager = manager;
        update();
    }

    void update();  

    bool contains(const sf::Vector2f& point) const;
    void draw(sf::RenderWindow& window) const;

private:
    void createTable();
};
