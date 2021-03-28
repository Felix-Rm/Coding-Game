#pragma once

#include "drawable.h"

class Button : public Drawable {
   private:
    void copyFrom(Button &other);

   protected:
    Window::event_handler_t handler;

    sf::RectangleShape background;

    bool highlighted = false;
    sf::Color bg_color;

   public:
    Button() : Drawable(nullptr, {0, 0}, {0, 0}) {}
    Button(Window *window, sf::Vector2f pos, sf::Vector2f size, int outline_thickness, sf::Color bg_color, Window::event_handler_t handler);

    Button(Button &&other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);
    }

    Button(Button &other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);
    }

    Button &operator=(Button &&other) {
        Drawable::operator=(std::move(other));

        copyFrom(other);

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        return *this;
    }

    ~Button();

    Button &center();

    void render() override;

    void setBgColor(sf::Color c);
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};