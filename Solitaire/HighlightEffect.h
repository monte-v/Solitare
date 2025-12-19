#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdint> 

class HighlightEffect {
private:
    struct Highlight {
        sf::RectangleShape rectangle;
        sf::Time duration;
        sf::Time elapsed;
        bool active;
    };

    std::vector<Highlight> highlights;
    sf::Color highlightColor;

public:
    HighlightEffect();

    void addHighlight(const sf::Vector2f& position, const sf::Vector2f& size, float durationSeconds = 2.0f);
    void update(sf::Time deltaTime);
    void draw(sf::RenderTarget& target) const;
    void clear();
    bool isEmpty() const;
};