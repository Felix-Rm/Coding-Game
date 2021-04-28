#pragma once

#include "button.h"

class TextButton : public Button {
   private:
    std::string text;
    sf::Color text_color = GameStyle::LIGHT_GRAY;
    float text_size;

    sf::Text text_sprite;

    void copyFrom(const TextButton &other);
    void loadText();

   public:
    TextButton() : Button() {}
    TextButton(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size);

    TextButton(TextButton &&other) : Button(std::move(other)) {
        copyFrom(other);
        loadText();
    };

    TextButton(const TextButton &other) : Button(other) {
        copyFrom(other);
        loadText();
    };

    TextButton &operator=(TextButton &&other) {
        Button::operator=(std::move(other));
        copyFrom(other);
        loadText();
        return *this;
    }

    TextButton &operator=(const TextButton &other) {
        Button::operator=(other);
        copyFrom(other);
        loadText();
        return *this;
    }

    ~TextButton();

    TextButton &center();

    void setTextColor(sf::Color c) {
        text_color = c;
        loadText();
    }

    void render() override;
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};