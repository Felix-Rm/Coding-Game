#pragma once

#include "drawable.h"

class Text : public Drawable {
   public:
    Text(){};

    Text(Window* window, sf::Vector2f pos, sf::Vector2f size, std::string text, int text_size, sf::Color color)
        : Drawable(window, pos, size) {
        this->text_element = sf::Text(text, GameStyle::game_font, text_size);
        this->text_element.setPosition(this->pos);
        this->text_element.setFillColor(color);
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

    void center() {
        auto bounds = this->text_element.getGlobalBounds();
        this->text_element.setPosition(this->pos.x + (this->size.x - bounds.width) / 2, this->pos.y);
    }

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
};