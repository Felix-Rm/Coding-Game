#pragma once

#include "window.h"

extern sf::Font game_font;

class Drawable {
   protected:
    sf::Vector2f pos;
    sf::Vector2f size;
    Window* window;

   public:
    Drawable(Window* window, sf::Vector2f pos, sf::Vector2f size) {
        this->window = window;
        this->pos = pos;
        this->size = size;
    }

    virtual ~Drawable(){};

    virtual sf::FloatRect getLocalBounds() {
        return sf::FloatRect({0, 0}, size);
    }

    virtual void setPosition(float x, float y) = 0;
    virtual void shiftPosition(float dx, float dy) = 0;

    virtual void render() = 0;
};