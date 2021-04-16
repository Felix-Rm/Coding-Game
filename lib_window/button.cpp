#include "button.h"

Button::Button(Window *window, sf::Vector2f pos, sf::Vector2f size) : Drawable(window, pos, size) {
    for (int i = 0; i < 3; i++)
        handler[i] = Window::createEventHandler(nullptr, Window::event_noop);

    this->bg_color = GameStyle::ORANGE;

    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setOutlineThickness(1);
    this->background.setFillColor(bg_color);
    background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});

    this->window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    this->window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);
}

Button::~Button() {
    this->window->removeEventHandler(onMouseMove, this);
    this->window->removeEventHandler(onMousePress, this);
}

void Button::copyFrom(const Button &other) {
    for (int i = 0; i < 3; i++)
        handler[i] = other.handler[i];

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
        obj->handler[event.mouseButton.button].ptr(event, obj->handler[event.mouseButton.button].data);

        return true;
    }

    return false;
}

void Button::setPosition(float x, float y) {
    this->pos = {x, y};
    this->background.setPosition(this->pos);
}

void Button::shiftPosition(float dx, float dy) {
    this->pos.x += dx;
    this->pos.y += dy;
    this->background.move(dx, dy);
}