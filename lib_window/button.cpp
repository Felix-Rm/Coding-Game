#include "button.h"

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler) : Drawable(pos, size) {
    this->handler = handler;

    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setFillColor(bg_color);

    this->text = sf::Text(text, game_font, text_size);
    this->text.setFillColor(fg_color);

    sf::FloatRect text_bounds = this->text.getLocalBounds();
    this->text.setPosition({pos.x + size.x / 2 - text_bounds.width / 2, pos.y + size.y / 2 - text_bounds.height * 0.8});
}

void Button::render(Window* window) {
    window->draw(background);
    window->draw(text);
}

Button& Button::center() {
    sf::Vector2f button_size = background.getSize();

    sf::Vector2f background_pos = background.getPosition();
    background.setPosition({background_pos.x - button_size.x / 2, background_pos.y - button_size.y / 2});

    sf::Vector2f text_pos = text.getPosition();
    text.setPosition({text_pos.x - button_size.x / 2, text_pos.y - button_size.y / 2});

    return *this;
}