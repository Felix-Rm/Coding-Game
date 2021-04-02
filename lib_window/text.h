#pragma once

#include "drawable.h"

class Text : public Drawable {
   public:
    Text();

    Text(Window* window, sf::Vector2f pos, sf::Vector2f size, std::string text, int text_size, sf::Color color)
        : Drawable(window, pos, size),
          text(text),
          color(color),
          text_size(text_size) {
        this->text_element = sf::Text(this->text, GameStyle::game_font, this->text_size);
        this->text_element.setPosition(this->pos);
        this->text_element.setFillColor(this->color);
    }

    Text(Text&& other) : Drawable(other) {
        this->text_element = other.text_element;
    }
    Text(Text& other) : Drawable(other) {
        this->text_element = other.text_element;
    }

    Text& operator=(Text&& other) {
        Drawable::operator=(std::move(other));
        this->text_element = other.text_element;

        return *this;
    }
    ~Text(){};

    void render() override {
        window->draw(this->text_element);
    }

    void setPosition(float x, float y) override {
        Drawable::setPosition(x, y);
        text_element.setPosition(this->pos);
    }

    void shiftPosition(float dx, float dy) override {
        Drawable::shiftPosition(dx, dy);
        text_element.setPosition(this->pos);
    }

   private:
    sf::Text text_element;

    std::string text;
    sf::Color color;
    int text_size;
};