#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class HelpWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<std::string> helpText;
    bool isOpenFlag;

public:
    HelpWindow();

    void open();
    void close();
    bool isOpen() const { return isOpenFlag; }

    void processEvents();
    void render();

private:
    void initializeText();
};