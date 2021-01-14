#include "stage.h"

Stage::Stage(Window *window, int stage_number, float scale) : Drawable({0, 0}, (sf::Vector2f)window->getSize())
{
    this->path = "assets/stages/" + std::to_string(stage_number) + '/';

    printf("%s\n", this->path.c_str());

    this->background_tex.loadFromFile(this->path + "background.png");
    this->background = sf::Sprite(this->background_tex);

    this->background.setScale({scale, scale});

    std::ifstream level_info(this->path + "level.info");
    level_info >> this->num_buttons;
    level_info.close();

    auto stage_size = this->background.getLocalBounds();

    int num_rows = 2;
    int num_columns = 5;
    int button_spacing = stage_size.width * 0.1;
    float button_size = stage_size.width * 0.05;
    int button_text_size = button_size * 0.8;
    int button_outline_size = button_size * 0.01;

    for (int i = 0; i < num_buttons; i++)
    {
        int col_num = i % num_columns;
        int row_num = i / num_rows;

        float x = stage_size.width / 2.0 - button_spacing * ((num_columns / 2) - col_num);
        float y = stage_size.height / 2.0 - button_spacing * ((num_rows / 2) + row_num - 0.5);

        char title[5];
        sprintf(title, "%d_%02d", stage_number, i);

        level_buttons.push_back(new Button(window, {x, y}, {button_size, button_size}, title, button_text_size, button_outline_size, game_colors::BLACK, game_colors::RED, Window::createEventHandler(Window::noop, nullptr)));
    }
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage()
{
    for (int i = 0; i < this->level_buttons.size(); i++)
        delete this->level_buttons[i];

    printf("stage %p deleted\n", this);
};

void Stage::setYOffset(int offset)
{
    this->background.setPosition(0, offset);
}

void Stage::render(Window *window)
{
    window->draw(this->background);

    for (int i = 0; i < this->level_buttons.size(); i++)
        this->level_buttons[i]->render(window);
}