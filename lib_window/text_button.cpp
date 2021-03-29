#include "text_button.h"

TextButton::TextButton(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler)
    : Button(window, pos, size, outline_thickness, bg_color, handler) {
    this->text = text;
    this->text_size = text_size;
    this->text_color = fg_color;

    loadText();
}

TextButton::~TextButton() {
}

void TextButton::loadText() {
    this->text_sprite = sf::Text(text, GameStyle::game_font, text_size);
    this->text_sprite.setFillColor(text_color);
    this->text_sprite.setStyle(sf::Text::Bold);
    sf::FloatRect text_bounds = this->text_sprite.getLocalBounds();
    this->text_sprite.setPosition({pos.x - text_bounds.left + size.x / 2 - text_bounds.width / 2, pos.y - text_bounds.top + size.y / 2 - text_bounds.height / 2});
}

void TextButton::copyFrom(TextButton &other) {
    text = other.text;
    text_size = other.text_size;
    text_color = other.text_color;
}

void TextButton::render() {
    this->window->draw(background);
    this->window->draw(text_sprite);
}

TextButton &TextButton::center() {
    sf::Vector2f button_size = background.getSize();
    pos.x -= button_size.x / 2;
    pos.y -= button_size.y / 2;

    background.setPosition(pos);

    text_sprite.move({-button_size.x / 2, -button_size.y / 2});

    return *this;
}

void TextButton::setPosition(float x, float y) {
    Button::setPosition(x, y);

    loadText();
}

void TextButton::shiftPosition(float dx, float dy) {
    Button::shiftPosition(dx, dy);
    this->text_sprite.move(dx, dy);
}