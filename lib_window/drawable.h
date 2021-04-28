#pragma once

#include "window.h"

extern sf::Font game_font;

class Drawable {
   protected:
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Vector2f orig_size;

    Window* window;

    void copyFrom(const Drawable& other) {
        pos = other.pos;
        size = other.size;
        window = other.window;
        orig_size = other.orig_size;
    }

   public:
    Drawable(){};

    Drawable(Window* window, sf::Vector2f pos, sf::Vector2f size) {
        this->window = window;
        this->pos = pos;
        this->orig_size = size;
        this->size = this->orig_size;
    }

    Drawable(Drawable&& other) {
        copyFrom(other);
    }

    Drawable(const Drawable& other) {
        copyFrom(other);
    }

    Drawable& operator=(Drawable&& other) {
        copyFrom(other);
        return *this;
    }

    Drawable& operator=(const Drawable& other) {
        copyFrom(other);
        return *this;
    }

    virtual ~Drawable(){};

    virtual sf::FloatRect getLocalBounds() {
        return sf::FloatRect({0, 0}, size);
    }

    virtual void setPosition(float x, float y) {
        pos.x = x;
        pos.y = y;
    };
    virtual void shiftPosition(float dx, float dy) {
        pos.x += dx;
        pos.y += dy;
    };

    virtual void setScale(float s) {
        this->size.x = orig_size.x * s;
        this->size.y = orig_size.y * s;
    }

    virtual void render() = 0;

    Window& getWindow() { return *window; };
};