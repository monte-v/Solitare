#include "StartMenu.h"
#include <iostream>

StartMenu::StartMenu() : visible(true), showRecords(false) {}

bool StartMenu::initialize(const std::string& fontPath, const std::string& texturePath) {
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Не удалось загрузить шрифт: " << fontPath << std::endl;
        return false;
    }

    if (!texturePath.empty()) {
        backgroundTexture.emplace();
        if (!backgroundTexture->loadFromFile(texturePath)) {
            std::cerr << "Не удалось загрузить текстуру фона: " << texturePath << std::endl;
            backgroundTexture.reset(); 
        }
        else {
            backgroundSprite.emplace(*backgroundTexture); 
            backgroundSprite->setScale({
                1000.0f / backgroundTexture->getSize().x,
                800.0f / backgroundTexture->getSize().y
                });
        }
    }

    createUX();

    recordsTable.initialize(font);

    return true;
}

void StartMenu::createUX() {
    startButton = Button("Начать игру", { 350.0f, 250.0f }, { 300.0f, 60.0f }, font);
    recordsButton = Button("Рекорды", { 350.0f, 330.0f }, { 300.0f, 60.0f }, font);
    helpButton = Button("Правила", { 350.0f, 410.0f }, { 300.0f, 60.0f }, font);
    exitButton = Button("Выход", { 350.0f, 490.0f }, { 300.0f, 60.0f }, font);

    title = std::make_unique<sf::Text>(font);
    title->setString("КОСЫНКА");
    title->setCharacterSize(48);
    title->setFillColor(sf::Color::White);
    title->setPosition({ 400.0f, 120.0f });
}

void StartMenu::handleClick(const sf::Vector2f& mousePos) {
    if (!visible) return;

    if (showRecords) {
        if (!recordsTable.contains(mousePos)) {
            hideRecordsTable();
        }
        return;
    }
    if (startButton.contains(mousePos)) {
        if (onStartCallback) {
            onStartCallback();
        }
    }
    else if (recordsButton.contains(mousePos)) {
        showRecordsTable();  
        if (onRecordsCallback) {
            onRecordsCallback();  
        }
    }
    else if (helpButton.contains(mousePos)) {
        if (onHelpCallback) {
            onHelpCallback();
        }
    }
    else if (exitButton.contains(mousePos)) {
        if (onExitCallback) {
            onExitCallback();
        }
    }
}

void StartMenu::draw(sf::RenderWindow& window) const {
    if (!visible) return;

    if (backgroundSprite.has_value()) {
        window.draw(*backgroundSprite);
    }
    else {
        window.clear(sf::Color(1, 62, 46));
    }

    if (!showRecords) {
        window.draw(*title);
        startButton.draw(window);
        recordsButton.draw(window);
        helpButton.draw(window);
        exitButton.draw(window);
    }

    if (showRecords) {
        recordsTable.draw(window);
    }
}

void StartMenu::updateRecordsTable() {
    recordsTable.update();
}

void StartMenu::setRecordsManager(RecordsManager* manager) {
    recordsTable.setRecordsManager(manager);
    recordsTable.update();
}

void StartMenu::setOnStart(const std::function<void()>& callback) {
    onStartCallback = callback;
}

void StartMenu::setOnRecords(const std::function<void()>& callback) {
    onRecordsCallback = callback;
}

void StartMenu::setOnHelp(const std::function<void()>& callback) {
    onHelpCallback = callback;
}

void StartMenu::setOnExit(const std::function<void()>& callback) {
    onExitCallback = callback;
}