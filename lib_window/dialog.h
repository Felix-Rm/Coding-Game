#pragma once

#include "drawable.h"
#include "image.h"
#include "text.h"
#include "text_button.h"

class Dialog : public Drawable {
   public:
    Dialog();
    Dialog(Window* window, sf::Vector2f pos, sf::Vector2f size, int outline_thickness, sf::Color bg_color);

    ~Dialog();

    void render() override;

    void addItem(Drawable* item) {
        items.push_back(item);
    }

    void addText(sf::Vector2f pos, std::string text, float text_size, sf::Color color, bool center = false) {
        Text* text_elt = new Text(window, {this->pos.x + pos.x, this->pos.y + pos.y}, {this->size.x, text_size}, text, text_size, color);
        if (center) text_elt->center();
        items.push_back(text_elt);
    }

    void addImage(sf::Vector2f pos, sf::Vector2f size, sf::Texture* tex, bool center = false) {
        Image* image_elt = new Image(window, {this->pos.x + pos.x, this->pos.y + pos.y}, size, tex);
        if (center) image_elt->center();
        items.push_back(image_elt);
    }

    TextButton* addButton(sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, bool center = false) {
        TextButton* button_elt = new TextButton(window, {this->pos.x + pos.x, this->pos.y + pos.y}, size, text, text_size);
        button_elt->setOutline(outline_thickness);
        button_elt->setTextColor(fg_color);
        button_elt->setBgColor(bg_color);

        if (center) button_elt->center();
        items.push_back(button_elt);

        return (TextButton*)items.back();
    }

    Dialog(Dialog&& other) : Drawable(other) {
        copyFrom(other);
    }

    Dialog(Dialog& other) : Drawable(other) {
        copyFrom(other);
    }

    Dialog& operator=(Dialog&& other) {
        Drawable::operator=(std::move(other));
        copyFrom(other);

        return *this;
    }

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

   protected:
    std::vector<Drawable*> items;

    void copyFrom(Dialog& other);
    void loadBackground();

    sf::RectangleShape background;
};