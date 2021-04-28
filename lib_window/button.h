#pragma once

#include "drawable.h"

class Button : public Drawable {
   public:
    Button() : Drawable(nullptr, {0, 0}, {0, 0}) {}
    Button(Window *window, sf::Vector2f pos, sf::Vector2f size);

    Button(Button &&other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);

        other.unregisterEvents();
        registerEvents();
    }

    Button(const Button &other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);

        registerEvents();
    }

    Button &operator=(Button &&other) {
        Drawable::operator=(std::move(other));

        copyFrom(other);

        other.unregisterEvents();
        registerEvents();

        return *this;
    }

    Button &operator=(const Button &other) {
        Drawable::operator=(other);

        copyFrom(other);

        registerEvents();

        return *this;
    }

    ~Button();

    void setOutline(int thickness) { background.setOutlineThickness(thickness); }
    void setBgColor(sf::Color c) {
        bg_color = c;
        background.setFillColor(c);
        background.setOutlineColor({(sf::Uint8)(c.r * 0.5), (sf::Uint8)(c.g * 0.5), (sf::Uint8)(c.b * 0.5)});
    }

    void setEventHandler(sf::Mouse::Button button, Window::event_handler_t event_handler) {
        if (button < 3) handler[button] = event_handler;
    }

    Button &center();

    void render() override;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);

   protected:
    Window::event_handler_t handler[3];

    sf::RectangleShape background;

    bool highlighted = false;
    sf::Color bg_color;

   private:
    void registerEvents();
    void unregisterEvents();
    void copyFrom(const Button &other);
};