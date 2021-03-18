#pragma once

#include "drawable.h"

class Button : public Drawable {
   private:
    Window::event_handler_t handler;

    sf::RectangleShape background;
    sf::Text text;

    bool highlighted = false;
    sf::Color bg_color;

   public:
    Button() : Drawable(nullptr, {0, 0}, {0, 0}) {}
    Button(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    Button(Button &&other) : Drawable(other.window, other.pos, other.size) {
        handler = other.handler;
        background = other.background;
        text = other.text;
        highlighted = other.highlighted;
        bg_color = other.bg_color;

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        printf("move %p to %p\n", &other, this);
    }

    Button(Button &other) : Drawable(other.window, other.pos, other.size) {
        handler = other.handler;
        background = other.background;
        text = other.text;
        highlighted = other.highlighted;
        bg_color = other.bg_color;

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        printf("copy to %p\n", this);
    }

    Button &operator=(Button &&other) {
        window = other.window;
        pos = other.pos;
        size = other.size;
        handler = other.handler;
        background = other.background;
        text = other.text;
        highlighted = other.highlighted;
        bg_color = other.bg_color;

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        printf("move op to %p\n", this);
        return *this;
    }

    ~Button();

    Button &center();

    void render() override;
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};