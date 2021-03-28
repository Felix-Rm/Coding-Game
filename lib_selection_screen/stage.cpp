#include "stage.h"

Stage::Stage(Window *window, std::string path, int stage_number, float scale, Window::event_handler_fnk_t event_handler) : Drawable(window, {0, 0}, (sf::Vector2f)window->getSize()) {
    this->path = path + std::to_string(stage_number) + '/';

    printf("%s\n", this->path.c_str());

    if (!this->background_tex.loadFromFile(this->path + "background.png"))
        throw std::runtime_error("[Stage] Could not open background.png: " + this->path + "background.png");

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

    level_executor_info.reserve(num_buttons);
    level_buttons.reserve(num_buttons);

    for (int i = 0; i < num_buttons; i++) {
        int col_num = i % num_columns;
        int row_num = i / num_columns;

        float x = -button_width / 2.0 + stage_size.width / 2.0 - button_spacing_x * ((num_columns / 2.0) - col_num - 0.5);
        float y = -button_height / 2.0 + stage_size.height / 2.0 - button_spacing_y * ((num_rows / 2.0) - row_num - 0.5);

        char title[5];
        snprintf(title, sizeof(title), "%dx%02x", stage_number, i);

        level_executor_info.push_back({this, i});
        level_buttons.push_back(TextButton(window, {x, y}, {(float)button_width, (float)button_height}, title, button_text_size, button_outline_size, GameStyle::BLACK, GameStyle::RED, Window::createEventHandler(event_handler, &(level_executor_info[i]))));

        printf("[Stage%d]: x: %f; y: %f; (i: %d)\n", stage_number, x, y, i);
    }
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage() {
    printf("stage %p deleted\n", this);
};

void Stage::setPosition(float x, float y) {
    sf::Vector2f delta = this->pos;
    delta.x -= x;
    delta.y -= y;

    this->pos = {x, y};

    this->background.setPosition(x, y);

    for (int i = 0; i < num_buttons; i++)
        level_buttons[i].shiftPosition(-delta.x, -delta.y);
}

void Stage::render() {
    this->window->draw(this->background);

    for (size_t i = 0; i < this->level_buttons.size(); i++)
        this->level_buttons[i].render();
}