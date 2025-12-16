//#pragma once
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <memory>
//
//class HighlightEffect {
//private:
//    struct Highlight {
//        sf::RectangleShape rectangle;
//        sf::Time duration;
//        sf::Time elapsed;
//    };
//
//    std::vector<Highlight> highlights;
//    sf::Color highlightColor;
//
//public:
//    HighlightEffect(const sf::Color& color = sf::Color::Yellow);
//
//    void addHighlight(const sf::FloatRect& bounds, float durationSeconds = 2.0f);
//    void addHighlight(const sf::Vector2f& position, const sf::Vector2f& size, float durationSeconds = 2.0f);
//    void update(sf::Time deltaTime);
//    void draw(sf::RenderTarget& target) const;
//    void clear();
//    bool isEmpty() const;
//};
//
//



#ifndef HIGHLIGHT_EFFECT_H
#define HIGHLIGHT_EFFECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdint> // Äëÿ uint8_t

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

#endif // HIGHLIGHT_EFFECT_H
