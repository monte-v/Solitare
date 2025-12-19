#include "RecordsTable.h"
#include <iomanip>
#include <sstream>

RecordsTable::RecordsTable() : visible(false), recordsManager(nullptr) {
    background.setFillColor(sf::Color(0, 0, 0, 220));
    tableBackground.setFillColor(sf::Color(30, 30, 50, 240));
    tableBackground.setOutlineColor(sf::Color::White);
    tableBackground.setOutlineThickness(2.0f);
}

void RecordsTable::initialize(sf::Font& font) {
    titleText.emplace(font);  
    titleText->setString("ËÓ×ØÈÅ ÐÅÇÓËÜÒÀÒÛ");
    titleText->setCharacterSize(32);
    titleText->setFillColor(sf::Color::White);
    titleText->setStyle(sf::Text::Bold);

    timeText.emplace(font);
    timeText->setString("ÂÐÅÌß");
    timeText->setCharacterSize(24);
    timeText->setFillColor(sf::Color::White);
    timeText->setStyle(sf::Text::Bold);

    timeTexts.clear();

    for (int i = 0; i < 5; i++) {
        std::optional<sf::Text> time;
        time.emplace(font);
        time->setCharacterSize(20);
        time->setFillColor(sf::Color::White);
        timeTexts.push_back(std::move(time));
    }

    setSize({ 600.0f, 500.0f });
    setPosition({ 200.0f, 150.0f });
}

void RecordsTable::setPosition(const sf::Vector2f& position) {
    background.setPosition(position);

    sf::Vector2f bgSize = background.getSize();
    sf::Vector2f tableSize = tableBackground.getSize();

    float tableX = position.x + (bgSize.x - tableSize.x) / 2.0f;
    float tableY = position.y + (bgSize.y - tableSize.y) / 2.0f;
    tableBackground.setPosition({ tableX, tableY });

    createTable();
}

void RecordsTable::setSize(const sf::Vector2f& size) {
    background.setSize(size);
    tableBackground.setSize({ size.x * 0.8f, size.y * 0.7f });
}

void RecordsTable::update() {
    if (!recordsManager) return;

    const auto& records = recordsManager->getRecords();

    for (size_t i = 0; i < 5; i++) {
        if (!timeTexts[i].has_value()) {
            continue;  
        }

        if (i < records.size()) {
            timeTexts[i]->setString(records[i].formattedTime);
            timeTexts[i]->setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
        }
        else {
            timeTexts[i]->setString("--:--.---");
            timeTexts[i]->setFillColor(sf::Color(100, 100, 100));
        }
    }

    createTable();
}

void RecordsTable::createTable() {
    if (!titleText.has_value() ||  !timeText.has_value()) {
        return;
    }

    sf::Vector2f tablePos = tableBackground.getPosition();
    sf::Vector2f tableSize = tableBackground.getSize();

    sf::FloatRect titleBounds = titleText->getLocalBounds();
    titleText->setPosition({
        tablePos.x + (tableSize.x - titleBounds.size.x) / 2.0f,
        tablePos.y + 20.0f
        });

    float timeTextX = tablePos.x + (tableSize.x - timeText->getLocalBounds().size.x) / 2 ;
    float headerY = tablePos.y + 80.0f;

    timeText->setPosition({ timeTextX, headerY });

    float rowHeight = 30.0f;  
    float startY = headerY + 40.0f;

    for (int i = 0; i < 5; i++) {

        if (!timeTexts[i].has_value()) {
            continue;
        }
        float rowX = tablePos.x + (tableSize.x - timeTexts[i]->getLocalBounds().size.x) / 2;
        float rowY = startY + i * rowHeight;

        timeTexts[i]->setPosition({ rowX , rowY });
    }
}

bool RecordsTable::contains(const sf::Vector2f& point) const {
    return background.getGlobalBounds().contains(point);
}

void RecordsTable::draw(sf::RenderWindow& window) const {
    if (!visible) return;

    window.draw(background);
    window.draw(tableBackground);

    if (titleText.has_value()) {
        window.draw(*titleText);
    }
    if (timeText.has_value()) {
        window.draw(*timeText);
    }

    for (const auto& text : timeTexts) {
        if (text.has_value()) {
            window.draw(*text);
        }
    }
}