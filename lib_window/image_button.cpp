#include "image_button.h"

ImageButton::ImageButton(Window *window, sf::Vector2f pos, sf::Vector2f size, GameStyle::Icon icon_id, float image_scale, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler) : Drawable(window, pos, size) {
    this->bg_color = bg_color;
    this->window = window;
    this->handler = handler;
    this->outline_thickness = outline_thickness;
    this->icon_id = icon_id;
    this->image_scale = image_scale;

    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setFillColor(bg_color);
    this->background.setOutlineThickness(outline_thickness);
    this->background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});

    this->window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    this->window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

    //printf("construct %p\n", this);
}

ImageButton::~ImageButton() {
    this->window->removeEventHandler(onMouseMove, this);
    this->window->removeEventHandler(onMousePress, this);
    //printf("destruct  %p\n", this);
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
    handler = other.handler;
    background = other.background;
    icon_id = other.icon_id;
    highlighted = other.highlighted;
    outline_thickness = other.outline_thickness;
    image_scale = other.image_scale;
    bg_color = other.bg_color;
}

void ImageButton::render() {
    this->window->draw(background);
    this->window->draw(image);
}

ImageButton &ImageButton::center() {
    sf::Vector2f button_size = background.getSize();
    pos.x -= button_size.x / 2;
    pos.y -= button_size.y / 2;

    background.setPosition(pos);

    image.move({-button_size.x / 2, -button_size.y / 2});

    return *this;
}

bool ImageButton::onMouseMove(sf::Event &event, void *_obj) {
    ImageButton *obj = (ImageButton *)_obj;

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

bool ImageButton::onMousePress(sf::Event &event, void *_obj) {
    ImageButton *obj = (ImageButton *)_obj;

    auto &mouseX = event.mouseButton.x;
    auto &mouseY = event.mouseButton.y;

    if (obj->background.getGlobalBounds().contains(mouseX, mouseY)) {
        obj->handler.ptr(event, obj->handler.data);

        return true;
    }

    return false;
}

void ImageButton::setPosition(float x, float y) {
    this->pos = {x, y};
    this->background.setPosition(this->pos);

    loadImage();
}

void ImageButton::shiftPosition(float dx, float dy) {
    this->pos.x += dx;
    this->pos.y += dy;
    this->background.move(dx, dy);
    this->image.move(dx, dy);
}