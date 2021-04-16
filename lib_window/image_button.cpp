#include "image_button.h"

ImageButton::ImageButton(Window *window, sf::Vector2f pos, sf::Vector2f size, GameStyle::Icon icon_id)
    : Button(window, pos, size) {
    this->icon_id = icon_id;

    loadImage();
}

ImageButton::~ImageButton() {
}

void ImageButton::loadImage() {
    this->image = sf::Sprite(GameStyle::icons[icon_id]);

    float scale = (this->size.y * image_scale) / this->image.getLocalBounds().height;
    this->image.setScale(scale, scale);

    sf::FloatRect image_bounds = this->image.getGlobalBounds();
    sf::Vector2f image_pos = {pos.x + size.x / 2 - image_bounds.width / 2,
                              pos.y + size.y / 2 - image_bounds.height / 2};
    this->image.setPosition(image_pos);
}

void ImageButton::copyFrom(ImageButton &other) {
    icon_id = other.icon_id;
    image_scale = other.image_scale;
}

void ImageButton::render() {
    this->window->draw(background);
    this->window->draw(image);
}

ImageButton &ImageButton::center() {
    Button::center();
    loadImage();

    return *this;
}

void ImageButton::setPosition(float x, float y) {
    Button::setPosition(x, y);
    loadImage();
}

void ImageButton::shiftPosition(float dx, float dy) {
    Button::shiftPosition(dx, dy);
    this->image.move(dx, dy);
}