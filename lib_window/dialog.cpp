#include "dialog.h"

Dialog::Dialog(Window* window, sf::Vector2f pos, sf::Vector2f size, int outline_thickness, sf::Color bg_color) : Drawable(window, pos, size) {
    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setFillColor(bg_color);
    this->background.setOutlineThickness(outline_thickness);
    this->background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});
};

Dialog::~Dialog(){

};

void Dialog::setPosition(float x, float y) {
    Drawable::setPosition(x, y);
    background.setPosition(this->pos);
}

void Dialog::shiftPosition(float x, float y) {
    Drawable::shiftPosition(x, y);
    background.setPosition(this->pos);
}

void Dialog::copyFrom(Dialog& other) {
    this->background = other.background;
}

void Dialog::render() {
    window->draw(this->background);

    for (auto& item : items) {
        item->render();
    }
}