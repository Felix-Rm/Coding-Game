#include "stage.h"
sf::Color Stage::progress_colors[4];

Stage::Stage(Window *window, std::string path, int stage_number, float scale, Window::event_handler_fnk_t event_handler) : Drawable(window, {0, 0}, (sf::Vector2f)window->getSize()) {
    progress_colors[0] = GameStyle::DARK_GRAY;
    progress_colors[1] = GameStyle::BRONZE;
    progress_colors[2] = GameStyle::GRAY;
    progress_colors[3] = GameStyle::GOLD;

    this->path = path + std::to_string(stage_number) + '/';
    this->level_creation_provider = event_handler;
    this->stage_number = stage_number;

    printf("%s\n", this->path.c_str());

    if (!this->background_tex.loadFromFile(this->path + "background.png"))
        throw std::runtime_error("[Stage] Could not open background.png: " + this->path + "background.png");

    this->background = sf::Sprite(this->background_tex);

    this->background.setScale({scale, scale});

    load();
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage() {
    printf("stage %p deleted\n", this);
};

void Stage::load() {
    std::ifstream stage_info(this->path + "stage.info");
    stage_info >> this->num_buttons;
    stage_info.close();

    stage_size = this->background.getGlobalBounds();

    num_rows = 3;
    num_columns = 6;

    button_spacing_x = stage_size.width / num_columns;
    button_spacing_y = stage_size.height / num_rows;

    button_width = stage_size.width * 0.1;
    button_height = button_width * 0.5;

    button_text_size = button_height * 0.6;
    button_outline_size = button_width * 0.05;

    level_executor_info.resize(num_buttons);
    level_buttons.resize(num_buttons);
    placeButtons();
}

void Stage::placeButtons() {
    for (int i = 0; i < num_buttons; i++) {
        char title[5];
        snprintf(title, sizeof(title), "%dx%02x", stage_number, i);

        bool a, b, c;
        int level_progress = LevelScreen::checkState("_user/gamesave/" + this->path + std::to_string(i) + "/completion_state.info", a, b, c);
        sf::Color button_color = progress_colors[level_progress];

        level_executor_info[i] = {this, i};
        level_buttons[i] = TextButton(window, {0, 0}, {(float)button_width, (float)button_height}, title, button_text_size);
        level_buttons[i].setOutline(button_outline_size);
        level_buttons[i].setTextColor(GameStyle::LIGHT_GRAY);
        level_buttons[i].setBgColor(button_color);
        level_buttons[i].setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(&(level_executor_info[i]), run_level));

        //printf("[Stage%d]: x: %f; y: %f; (i: %d)\n", stage_number, x, y, i);
    }

    setPosition(this->pos.x, this->pos.y);
}

void Stage::setPosition(float x, float y) {
    Drawable::setPosition(x, y);

    this->background.setPosition(x, y);

    for (int i = 0; i < num_buttons; i++) {
        int col_num = i % num_columns;
        int row_num = i / num_columns;
        float x = pos.x - button_width / 2.0 + stage_size.width / 2.0 - button_spacing_x * ((num_columns / 2.0) - col_num - 0.5);
        float y = pos.y - button_height / 2.0 + stage_size.height / 2.0 - button_spacing_y * ((num_rows / 2.0) - row_num - 0.5);

        level_buttons[i].setPosition(x, y);
    }

    if (btn_add) {
        int col_num = num_buttons % num_columns;
        int row_num = num_buttons / num_columns;
        float x = pos.x - button_width / 2.0 + stage_size.width / 2.0 - button_spacing_x * ((num_columns / 2.0) - col_num - 0.5);
        float y = pos.y - button_height / 2.0 + stage_size.height / 2.0 - button_spacing_y * ((num_rows / 2.0) - row_num - 0.5);

        btn_add->setPosition(x, y);
    }
}

void Stage::render() {
    this->window->draw(this->background);

    for (size_t i = 0; i < this->level_buttons.size(); i++)
        this->level_buttons[i].render();

    if (btn_add) btn_add->render();
}

bool Stage::run_level(sf::Event &event, void *data) {
    auto *info = (std::pair<Stage *, int> *)data;

    info->first->getWindow().setVisible(false);

    LevelScreen *window = nullptr;
    int current_level_id = info->second;

    while (true) {
        if (window != nullptr) delete window;

        level_creation_data creation_data;
        creation_data.level_id = current_level_id;
        creation_data.path = info->first->path;
        creation_data.level_window = (Window **)&window;
        creation_data.selection_window = &info->first->getWindow();

        info->first->level_creation_provider(event, &creation_data);
        window->next_level_exists = (info->second + 1) < info->first->getNumberOfLevels();

        while (window->run())
            ;

        if (window->next_level_flag)
            current_level_id++;
        else
            break;
    };

    delete window;

    info->first->placeButtons();
    info->first->getWindow().setVisible(true);
    return true;
}

bool Stage::addLevel(sf::Event &event, void *data) {
    Stage *obj = (Stage *)data;

    std::ofstream stage_info(obj->path + "stage.info");
    stage_info << obj->num_buttons + 1;
    stage_info.close();

    obj->load();
    return true;
}

void Stage::activateEditMode(bool a) {
    edit_mode_activated = a;
    if (edit_mode_activated) {
        if (num_buttons < num_columns * num_rows && !btn_add) {
            btn_add = new TextButton(window, {0, 0}, {button_width, button_height}, "+", button_text_size);
            btn_add->setOutline(button_outline_size);
            btn_add->setTextColor(GameStyle::LIGHT_GRAY);
            btn_add->setBgColor(GameStyle::GREEN);
            btn_add->setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, Stage::addLevel));
            setPosition(pos.x, pos.y);
        }

        for (int i = 0; i < num_buttons; i++) {
            button.setEventHandler(sf::Mouse::Button::Right, Window::createEventHandler(level_executor_info[i] ,Stage::deleteLevel);
        }
    }

    if (btn_add && (num_buttons >= num_columns * num_rows || !edit_mode_activated)) {
        delete btn_add;
        btn_add = nullptr;
    }
};