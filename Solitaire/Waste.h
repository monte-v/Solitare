#pragma once
#include "Pile.h"

class Waste : public Pile {
public:
    Waste(sf::Vector2f pos);

    // Waste имеет особые правила:
    // -  арты всегда видны (рубашкой вниз)
    // -  арты располагаютс€ горизонтально с небольшим смещением
    // - ћожно брать только верхнюю карту
    // - Ќельз€ класть карты обратно в Waste из других стопок

    bool canAddCard(const Card& card) const override;
    bool canTakeCard() const override { return !cards.empty(); }
    bool canTakeCardFrom(int index) const override;

    // ќсобый метод дл€ отрисовки с горизонтальным смещением
    void draw(sf::RenderTarget& target) const override;

private:
    static constexpr float HORIZONTAL_OFFSET = 15.0f; // —мещение по X
};