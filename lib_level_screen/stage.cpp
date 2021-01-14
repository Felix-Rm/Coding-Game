#include "stage.h"

Stage::Stage(Window* window, int stage_number, float scale) : Drawable({0, 0}, (sf::Vector2f)window->getSize()) {
    this->path = "assets/stages/" + std::to_string(stage_number) + '/';

    printf("%s\n", this->path.c_str());

    this->background_tex.loadFromFile(this->path + "background.png");
    this->background = sf::Sprite(this->background_tex);

    this->background.setScale({scale, scale});

    // this->background.setTexture(this->background_tex);
    // this->background.setTextureRect(sf::IntRect({0, 0}, (sf::Vector2i)this->background_tex.getSize()));
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage() {
    printf("stage %p deleted\n", this);
};

void Stage::setYOffset(int offset) {
    this->background.setPosition(0, offset);
}

void Stage::render(Window* window) {
    window->draw(this->background);
}