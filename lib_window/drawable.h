#pragma once

#include "window.h"

extern sf::Font game_font;

class Drawable {
   protected:
    sf::Vector2f pos;
    sf::Vector2f size;
    Window* window;

   public:
    Drawable(sf::Vector2f pos, sf::Vector2f size) {
        this->pos = pos;
        this->size = size;
    }

    virtual void render(Window* window) = 0;
};