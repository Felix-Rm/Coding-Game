#pragma once

#include "button.h"

class TextButton : public Button {
   private:
    sf::Text text;

    void copyFrom(Button &other);

   public:
    TextButton() : Button() {}
    TextButton(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    TextButton(TextButton &&other) : Button(other) {
        text = other.text;
    }

    TextButton(TextButton &other) : Button(other) {
        text = other.text;
    }
    TextButton &operator=(TextButton &&other) {
        Button::operator=(std::move(other));
        return *this;
    }

    ~TextButton();

    TextButton &center();

    void render() override;
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};