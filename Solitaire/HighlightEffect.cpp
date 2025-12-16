//#include "HighlightEffect.h"
//
//HighlightEffect::HighlightEffect(const sf::Color& color)
//    : highlightColor(color) {
//    highlightColor.a = 100; 
//}
//
//void HighlightEffect::addHighlight(const sf::FloatRect& bounds, float durationSeconds) {
//    Highlight highlight;
//    highlight.rectangle.setPosition(bounds.left, bounds.top);
//    highlight.rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
//    highlight.rectangle.setFillColor(sf::Color::Transparent);
//    highlight.rectangle.setOutlineColor(highlightColor);
//    highlight.rectangle.setOutlineThickness(3.0f);
//    highlight.duration = sf::seconds(durationSeconds);
//    highlight.elapsed = sf::Time::Zero;
//
//    highlights.push_back(highlight);
//}
//
//void HighlightEffect::addHighlight(const sf::Vector2f& position, const sf::Vector2f& size, float durationSeconds) {
//    Highlight highlight;
//    highlight.rectangle.setPosition(position);
//    highlight.rectangle.setSize(size);
//    highlight.rectangle.setFillColor(sf::Color::Transparent);
//    highlight.rectangle.setOutlineColor(highlightColor);
//    highlight.rectangle.setOutlineThickness(3.0f);
//    highlight.duration = sf::seconds(durationSeconds);
//    highlight.elapsed = sf::Time::Zero;
//
//    highlights.push_back(highlight);
//}
//
//void HighlightEffect::update(sf::Time deltaTime) {
//    for (auto it = highlights.begin(); it != highlights.end();) {
//        it->elapsed += deltaTime;
//
//        // Плавное исчезновение
//        float alpha = 100.0f * (1.0f - it->elapsed.asSeconds() / it->duration.asSeconds());
//        alpha = std::max(0.0f, std::min(100.0f, alpha));
//
//        sf::Color color = highlightColor;
//        color.a = static_cast<sf::Uint8>(alpha);
//        it->rectangle.setOutlineColor(color);
//
//        if (it->elapsed >= it->duration) {
//            it = highlights.erase(it);
//        }
//        else {
//            ++it;
//        }
//    }
//}
//
//void HighlightEffect::draw(sf::RenderTarget& target) const {
//    for (const auto& highlight : highlights) {
//        target.draw(highlight.rectangle);
//    }
//}
//
//void HighlightEffect::clear() {
//    highlights.clear();
//}
//
//bool HighlightEffect::isEmpty() const {
//    return highlights.empty();
//}


#include "HighlightEffect.h"
#include <algorithm>

HighlightEffect::HighlightEffect() {
    highlightColor = sf::Color::Yellow;
    highlightColor.a = 150; // Полупрозрачный
}

void HighlightEffect::addHighlight(const sf::Vector2f& position, const sf::Vector2f& size, float durationSeconds) {
    Highlight highlight;
    highlight.rectangle.setPosition(position);
    highlight.rectangle.setSize(size);
    highlight.rectangle.setFillColor(sf::Color::Transparent);
    highlight.rectangle.setOutlineColor(highlightColor);
    highlight.rectangle.setOutlineThickness(3.0f);
    highlight.duration = sf::seconds(durationSeconds);
    highlight.elapsed = sf::Time::Zero;
    highlight.active = true;

    highlights.push_back(highlight);
}

void HighlightEffect::update(sf::Time deltaTime) {
    for (size_t i = 0; i < highlights.size();) {
        highlights[i].elapsed += deltaTime;

        // Плавное исчезновение
        float progress = highlights[i].elapsed.asSeconds() / highlights[i].duration.asSeconds();
        progress = std::min(1.0f, progress);

        uint8_t alpha = static_cast<uint8_t>(150 * (1.0f - progress));
        sf::Color color = highlightColor;
        color.a = alpha;
        highlights[i].rectangle.setOutlineColor(color);

        if (progress >= 1.0f) {
            highlights.erase(highlights.begin() + i);
        }
        else {
            ++i;
        }
    }
}

void HighlightEffect::draw(sf::RenderTarget& target) const {
    for (const auto& highlight : highlights) {
        target.draw(highlight.rectangle);
    }
}

void HighlightEffect::clear() {
    highlights.clear();
}

bool HighlightEffect::isEmpty() const {
    return highlights.empty();
}