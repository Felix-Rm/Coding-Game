#pragma once

#include "window.h"

class Drawable {
   private:
    sf::Vector2f pos;
    sf::Vector2f size;

   public:
    Drawable(sf::Vector2f pos, sf::Vector2f size) {
        this->pos = pos;
        this->size = size;
    }

    virtual void render(Window* window) = 0;
};