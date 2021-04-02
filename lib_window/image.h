#pragma once

#include "drawable.h"

class Image : public Drawable {
   public:
    Image(){};

    Image(Window* window, sf::Vector2f pos, sf::Vector2f size, sf::Texture* tex)
        : Drawable(window, pos, size) {
        this->tex = tex;
        loadImage();
    }

    Image(Image&& other) : Drawable(other) {
        this->tex = other.tex;
        loadImage();
    }

    Image(Image& other) : Drawable(other) {
        this->tex = other.tex;
        loadImage();
    }

    Image& operator=(Image&& other) {
        Drawable::operator=(std::move(other));
        this->tex = other.tex;
        loadImage();

        return *this;
    }
    ~Image(){};

    void center() {
        auto bounds = this->image.getGlobalBounds();
        this->image.setPosition(this->pos.x + (this->size.x - bounds.width) / 2, this->pos.y);
    }

    void render() override {
        window->draw(this->image);
    }

    void setPosition(float x, float y) override {
        Drawable::setPosition(x, y);
        image.setPosition(this->pos);
    }

    void shiftPosition(float dx, float dy) override {
        Drawable::shiftPosition(dx, dy);
        image.setPosition(this->pos);
    }

    void setScale(float s) override {
        Drawable::setScale(s);
        image.setScale(s, s);
    }

   private:
    void loadImage() {
        this->image = sf::Sprite(*(this->tex));
        this->image.setPosition(this->pos);
        auto bounds = this->image.getGlobalBounds();
        this->image.setScale(size.x / bounds.width, size.y / bounds.height);
    }

    sf::Sprite image;
    sf::Texture* tex;
};