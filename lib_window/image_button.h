#pragma once

#include "button.h"

class ImageButton : public Button {
   private:
    sf::Texture texture;
    sf::Sprite image;

    float image_scale;
    GameStyle::Icon icon_id;

    void loadImage();

    void copyFrom(ImageButton &other);

   public:
    ImageButton() : Button() {}
    ImageButton(Window *window, sf::Vector2f pos, sf::Vector2f size, GameStyle::Icon icon_id, float image_scale, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler);

    ImageButton(ImageButton &&other) : Button(other) {
        copyFrom(other);
        loadImage();
    }

    ImageButton(ImageButton &other) : Button(other) {
        copyFrom(other);
        loadImage();
    }

    ImageButton &operator=(ImageButton &&other) {
        Button::operator=(std::move(other));

        copyFrom(other);
        loadImage();
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