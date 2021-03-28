#include "button.h"

Button::Button(Window *window, sf::Vector2f pos, sf::Vector2f size, int outline_thickness, sf::Color bg_color, Window::event_handler_t handler) : Drawable(window, pos, size) {
    this->bg_color = bg_color;
    this->handler = handler;

    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setFillColor(bg_color);
    this->background.setOutlineThickness(outline_thickness);
    this->background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});

    this->window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    this->window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);
}

Button::~Button() {
    this->window->removeEventHandler(onMouseMove, this);
    this->window->removeEventHandler(onMousePress, this);
}

void Button::copyFrom(Button &other) {
    handler = other.handler;
    background = other.background;
    highlighted = other.highlighted;
    bg_color = other.bg_color;
}

void Button::render() {
    this->window->draw(background);
}

Button &Button::center() {
    sf::Vector2f button_size = background.getSize();
    pos.x -= button_size.x / 2;
    pos.y -= button_size.y / 2;

    background.setPosition(pos);

    return *this;
}

bool Button::onMouseMove(sf::Event &event, void *_obj) {
    Button *obj = (Button *)_obj;

    auto &mouseX = event.mouseMove.x;
    auto &mouseY = event.mouseMove.y;

    if (obj->background.getGlobalBounds().contains(mouseX, mouseY) && !obj->highlighted) {
        obj->highlighted = true;
        obj->background.setFillColor(obj->background.getOutlineColor());

        return true;
    }

    if (!obj->background.getGlobalBounds().contains(mouseX, mouseY) && obj->highlighted) {
        obj->highlighted = false;
        obj->background.setFillColor(obj->bg_color);
    }

    return false;
}

bool Button::onMousePress(sf::Event &event, void *_obj) {
    Button *obj = (Button *)_obj;

    auto &mouseX = event.mouseButton.x;
    auto &mouseY = event.mouseButton.y;

    if (obj->background.getGlobalBounds().contains(mouseX, mouseY)) {
        obj->handler.ptr(event, obj->handler.data);

        return true;
    }

    return false;
}

void Button::setBgColor(sf::Color c) {
    this->bg_color = c;
    this->background.setFillColor(bg_color);
    this->background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});
};

void Button::setPosition(float x, float y) {
    this->pos = {x, y};
    this->background.setPosition(this->pos);
}

void Button::shiftPosition(float dx, float dy) {
    this->pos.x += dx;
    this->pos.y += dy;
    this->background.move(dx, dy);
}