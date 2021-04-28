#include "stage.h"
sf::Color Stage::progress_colors[4];

Stage::Stage(Window *window, std::string load_path, std::string save_path, int stage_number, float scale, Window::event_handler_fnk_t event_handler) : Drawable(window, {0, 0}, (sf::Vector2f)window->getSize()) {
    progress_colors[0] = GameStyle::DARK_GRAY;
    progress_colors[1] = GameStyle::BRONZE;
    progress_colors[2] = GameStyle::GRAY;
    progress_colors[3] = GameStyle::GOLD;

    this->load_path = load_path + std::to_string(stage_number) + '/';
    this->save_path = save_path + std::to_string(stage_number) + '/';

    this->level_creation_provider = event_handler;
    this->stage_number = stage_number;

    if (!this->background_tex.loadFromFile(this->load_path + "background.png"))
        throw std::runtime_error("[Stage] Could not open background.png: " + this->load_path + "background.png");

    this->background = sf::Sprite(this->background_tex);

    this->background.setScale({scale, scale});

    load();
}

// Stage::Stage() : Drawable({0, 0}, {0, 0}){};
Stage::~Stage() {
    window->removeEventHandler(contextMenu, &level_executor_info[num_buttons]);
    printf("stage %p deleted\n", this);
};

void Stage::load() {
    window->removeEventHandler(contextMenu, &level_executor_info[0]);

    std::ifstream stage_info(this->load_path + "stage.info");
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

    level_executor_info.clear();
    level_executor_info.resize(num_buttons + 1);

    level_buttons.clear();
    level_buttons.resize(num_buttons);

    placeButtons();

    if (num_buttons == 0)
        window->addEventHandler(contextMenu, &level_executor_info[0], 1, sf::Event::MouseButtonPressed);
}

void Stage::placeButtons() {
    for (int i = 0; i < num_buttons; i++) {
        char title[5];
        snprintf(title, sizeof(title), "%dx%02x", stage_number, i);

        bool a, b, c;
        int level_progress = LevelScreen::checkState(this->save_path + std::to_string(i) + "/completion_state.info", a, b, c);
        sf::Color button_color = progress_colors[level_progress];

        level_executor_info[i] = {this, i};
        level_buttons[i] = TextButton(window, {0, 0}, {(float)button_width, (float)button_height}, title, button_text_size);
        level_buttons[i].setOutline(button_outline_size);
        level_buttons[i].setTextColor(GameStyle::LIGHT_GRAY);
        level_buttons[i].setBgColor(button_color);
        level_buttons[i].setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(&(level_executor_info[i]), run_level));

        if (edit_mode_activated)
            level_buttons[i].setEventHandler(sf::Mouse::Button::Right, Window::createEventHandler(&level_executor_info[i], Stage::contextMenu));

        //printf("[Stage%d]: x: %f; y: %f; (i: %d)\n", stage_number, x, y, i);
    }

    level_executor_info[num_buttons] = {this, -1};

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

    if (context) {
        delete context;
        context = nullptr;
    }
}

void Stage::render() {
    this->window->draw(this->background);

    for (size_t i = 0; i < this->level_buttons.size(); i++)
        this->level_buttons[i].render();

    if (context) context->render();
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
        creation_data.load_path = info->first->load_path;
        creation_data.save_path = info->first->save_path;
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

bool Stage::contextMenu(sf::Event &event, void *data) {
    auto info = (std::pair<Stage *, int> *)data;
    Stage *obj = info->first;

    if (info->second == -1 && (!obj->background.getGlobalBounds().contains(obj->window->mouse_pos) || event.mouseButton.button != sf::Mouse::Button::Right)) {
        return false;
    }

    float padding = 10;
    int num_buttons = info->second == -1 ? 1 : 3;
    sf::Vector2f context_button_size = {260, 40};
    sf::Vector2f context_size = {context_button_size.x + padding * 2, (context_button_size.y + padding) * num_buttons + padding};

    sf::Vector2f button_pos = {padding, padding};

    if (obj->context) delete obj->context;
    obj->context = new Dialog(&obj->getWindow(), obj->window->mouse_pos, context_size, 2, GameStyle::DARK_GRAY);

    TextButton *btn;

    if (info->second == -1) {
        btn = obj->context->addButton(button_pos, context_button_size, "Add Level", context_button_size.y * 0.6, 2, GameStyle::LIGHT_GRAY, GameStyle::GREEN);
        btn->setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(data, addLevel));
        button_pos.y += context_button_size.y + padding;
    } else {
        btn = obj->context->addButton(button_pos, context_button_size, "Delete Level", context_button_size.y * 0.6, 2, GameStyle::LIGHT_GRAY, GameStyle::RED);
        btn->setEventHandler(sf::Mouse::Button::Middle, Window::createEventHandler(data, deleteLevel));
        button_pos.y += context_button_size.y + padding;

        btn = obj->context->addButton(button_pos, context_button_size, "Add Level (left)", context_button_size.y * 0.6, 2, GameStyle::LIGHT_GRAY, GameStyle::GREEN);
        btn->setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(data, addLevelLeft));
        button_pos.y += context_button_size.y + padding;

        btn = obj->context->addButton(button_pos, context_button_size, "Add Level (right)", context_button_size.y * 0.6, 2, GameStyle::LIGHT_GRAY, GameStyle::GREEN);
        btn->setEventHandler(sf::Mouse::Button::Middle, Window::createEventHandler(data, addLevelRight));
    }

    return true;
}

bool Stage::deleteLevel(sf::Event &event, void *data) {
    auto info = (std::pair<Stage *, int> *)data;
    auto obj = info->first;

    if (std::filesystem::exists(info->first->load_path + std::to_string(info->second)))
        std::filesystem::remove_all(info->first->load_path + std::to_string(info->second));

    if (std::filesystem::exists(info->first->save_path + std::to_string(info->second)))
        std::filesystem::remove_all(info->first->save_path + std::to_string(info->second));

    obj->shiftLevels(info->second + 1, obj->num_buttons, -1);

    std::ofstream stage_info(obj->load_path + "stage.info");
    stage_info << obj->num_buttons - 1;
    stage_info.close();

    obj->load();

    return true;
}

bool Stage::addLevel(sf::Event &event, void *data) {
    auto info = (std::pair<Stage *, int> *)data;
    auto obj = info->first;

    obj->shiftLevels(info->second + 1, obj->num_buttons, 1);

    std::ofstream stage_info(obj->load_path + "stage.info");
    stage_info << obj->num_buttons + 1;
    stage_info.close();

    obj->load();

    return true;
}

bool Stage::addLevelRight(sf::Event &event, void *data) {
    return addLevel(event, data);
}

bool Stage::addLevelLeft(sf::Event &event, void *data) {
    auto info = *(std::pair<Stage *, int> *)data;
    info.second--;
    return addLevel(event, &info);
}

void Stage::shiftLevels(int start, int end, int by) {
    for (int i = start; i < end; i++) {
        if (std::filesystem::exists(load_path + std::to_string(i)))
            std::filesystem::rename(load_path + std::to_string(i), load_path + std::to_string(i + by));

        if (std::filesystem::exists(save_path + std::to_string(i)))
            std::filesystem::rename(save_path + std::to_string(i), save_path + std::to_string(i + by));
    }
}

void Stage::activateEditMode(bool a) {
    edit_mode_activated = a;

    load();
};