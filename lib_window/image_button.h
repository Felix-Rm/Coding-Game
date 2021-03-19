#pragma once

#include "drawable.h"

class ImageButton : public Drawable {
   private:
    Window::event_handler_t handler;

    sf::RectangleShape background;
    sf::Texture texture;
    sf::Sprite image;

    float image_scale;
    int outline_thickness;
    bool highlighted = false;
    sf::Color bg_color;
    GameStyle::Icon icon_id;

    void loadImage();

    void copyFrom(ImageButton &other);

   public:
    ImageButton() : Drawable(nullptr, {0, 0}, {0, 0}) {}
    ImageButton(Window *window, sf::Vector2f pos, sf::Vector2f size, GameStyle::Icon icon_id, float image_scale, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    ImageButton(ImageButton &&other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);
        loadImage();

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        //printf("move %p to %p\n", &other, this);
    }

    ImageButton(ImageButton &other) : Drawable(other.window, other.pos, other.size) {
        copyFrom(other);
        loadImage();

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        //printf("copy to %p\n", this);
    }

    ImageButton &operator=(ImageButton &&other) {
        window = other.window;
        pos = other.pos;
        size = other.size;

        copyFrom(other);
        loadImage();

        window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
        window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);

        //printf("move op to %p\n", this);
        return *this;
    }

    ~ImageButton();

    ImageButton &center();

    void render() override;
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};