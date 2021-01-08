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
    Button(Window* window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);
    ~Button();

    Button& center();

    void render(Window* window) override;

    static bool onMouseMove(sf::Event& event, void*);
    static bool onMousePress(sf::Event& event, void*);
};