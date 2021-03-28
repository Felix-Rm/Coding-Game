#include "text_button.h"

TextButton::TextButton(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler)
    : Button(window, pos, size, outline_thickness, bg_color, handler) {
    this->text = sf::Text(text, GameStyle::game_font, text_size);
    this->text.setFillColor(fg_color);
    this->text.setStyle(sf::Text::Bold);

    sf::FloatRect text_bounds = this->text.getLocalBounds();
    this->text.setPosition({pos.x - text_bounds.left + size.x / 2 - text_bounds.width / 2, pos.y - text_bounds.top + size.y / 2 - text_bounds.height / 2});
}

TextButton::~TextButton() {
}

void TextButton::render() {
    this->window->draw(background);
    this->window->draw(text);
}

TextButton &TextButton::center() {
    sf::Vector2f button_size = background.getSize();
    pos.x -= button_size.x / 2;
    pos.y -= button_size.y / 2;

    background.setPosition(pos);

    text.move({-button_size.x / 2, -button_size.y / 2});

    return *this;
}

void TextButton::setPosition(float x, float y) {
    Button::setPosition(x, y);

    sf::FloatRect text_bounds = this->text.getLocalBounds();
    this->text.setPosition({x - text_bounds.left + size.x / 2 - text_bounds.width / 2, y - text_bounds.top + size.y / 2 - text_bounds.height / 2});
}

void TextButton::shiftPosition(float dx, float dy) {
    Button::shiftPosition(dx, dy);
    this->text.move(dx, dy);
}