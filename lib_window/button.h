#pragma once

#include "drawable.h"

extern sf::Font game_font;

class Button : public Drawable {
   private:
    Window::event_handler_t handler;

    sf::RectangleShape background;
    sf::Text text;

   public:
    Button(sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    Button& center();
    void render(Window* window) override;
};