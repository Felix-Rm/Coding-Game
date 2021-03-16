#include "stage.h"

Stage::Stage(Window *window, int stage_number, float scale) : Drawable({0, 0}, (sf::Vector2f)window->getSize()) {
    this->path = "assets/stages/" + std::to_string(stage_number) + '/';

    printf("%s\n", this->path.c_str());

    this->background_tex.loadFromFile(this->path + "background.png");
    this->background = sf::Sprite(this->background_tex);

    this->background.setScale({scale, scale});

    std::ifstream stage_info(this->path + "stage.info");
    stage_info >> this->num_buttons;
    stage_info.close();

    auto stage_size = this->background.getGlobalBounds();

    int num_rows = 3;
    int num_columns = 6;

    int button_spacing_x = stage_size.width / num_columns;
    int button_spacing_y = stage_size.height / num_rows;

    int button_width = stage_size.width * 0.1;
    int button_height = button_width * 0.5;

    int button_text_size = button_height * 0.6;
    int button_outline_size = button_width * 0.05;

    for (int i = 0; i < num_buttons; i++) {
        int col_num = i % num_columns;
        int row_num = i / num_columns;

        float x = -button_width / 2.0 + stage_size.width / 2.0 - button_spacing_x * ((num_columns / 2.0) - col_num - 0.5);
        float y = -button_height / 2.0 + stage_size.height / 2.0 - button_spacing_y * ((num_rows / 2.0) - row_num - 0.5);

        char title[5];
        sprintf(title, "%dx%02x", stage_number, i);

        Button *b = new Button(window, {x, y}, {(float)button_width, (float)button_height}, title, button_text_size, button_outline_size, GameStyle::BLACK, GameStyle::RED, Window::createEventHandler(Window::noop, nullptr));
        level_buttons.push_back(b);

        printf("[Stage%d]: x: %f; y: %f; (i: %d)\n", stage_number, x, y, i);
    }
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage() {
    for (int i = 0; i < this->level_buttons.size(); i++)
        delete this->level_buttons[i];

    printf("stage %p deleted\n", this);
};

void Stage::setPosition(float x, float y) {
    sf::Vector2f delta = this->pos;
    delta.x -= x;
    delta.y -= y;

    this->pos = {x, y};

    this->background.setPosition(x, y);

    for (int i = 0; i < num_buttons; i++)
        level_buttons[i]->shiftPosition(-delta.x, -delta.y);
}

void Stage::render(Window *window) {
    window->draw(this->background);

    for (int i = 0; i < this->level_buttons.size(); i++)
        this->level_buttons[i]->render(window);
}