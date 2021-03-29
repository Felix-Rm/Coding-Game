#pragma once

#include "button.h"

class TextButton : public Button {
   private:
    std::string text;
    sf::Color text_color;
    float text_size;

    sf::Text text_sprite;

    void copyFrom(TextButton &other);
    void loadText();

   public:
    TextButton() : Button() {}
    TextButton(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    TextButton(TextButton &&other) : Button(other) {
        copyFrom(other);
        loadText();
    };

    TextButton(TextButton &other) : Button(other) {
        copyFrom(other);
        loadText();
    };

    TextButton &operator=(TextButton &&other) {
        Button::operator=(std::move(other));
        copyFrom(other);
        loadText();
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