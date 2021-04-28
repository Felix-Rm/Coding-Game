#include "level_screen.h"

sf::Texture LevelScreen::star_textures[2][3];
bool LevelScreen::textures_loaded = false;

LevelScreen::LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &path) : Window(video_mode, title, style) {
    if (!textures_loaded)
        loadTextures();

    Tile::loadTextures();
    Bot::loadTextures();

    this->top_bar = new TopBar(this);

    this->path = path + std::to_string(level_num) + '/';

    if (!std::filesystem::exists(this->path + "level.info")) {
        printf("creating not existing level\n");
        std::filesystem::create_directories(this->path);
        std::ofstream level_info(this->path + "level.info");
        level_info << "1 1 1";
        level_info.close();
    }

    std::ifstream level_info(this->path + "level.info");
    if (!level_info)
        throw std::runtime_error("could not open level info");

    level_info >> *this;

    level_info.close();

    // for (int i = 0; i < num_bots; i++) {
    //     int x, y;
    //     level_info >> x >> y;
    //     bots.push_back(new Bot(this, {(float)x * Tile::tex_size.y, (float)y * Tile::tex_size.y}, {(float)x, (float)y}, level_info, this));
    // }

    addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    addEventHandler(onScroll, this, 1, sf::Event::MouseWheelMoved);
    addEventHandler(onMouseButton, this, 2, sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased);
}

std::ifstream &operator>>(std::ifstream &data, LevelScreen &obj) {
    data >> obj.size.x >> obj.size.y;
    obj.map.resize(obj.size.x);

    int level_height = obj.size.y * Tile::tex_size.y;
    int level_width = obj.size.x * Tile::tex_size.x;

    obj.scale = min(obj.view_size.x / level_width, obj.view_size.y / level_height);
    obj.origin.x = (obj.view_size.x - (level_width * obj.scale)) / 2;
    obj.origin.y = (obj.view_size.y - (level_height * obj.scale)) / 2;

    for (size_t x = 0; x < obj.size.x; x++) {
        //obj.map[x] = std::vector<Tile *>();
        obj.map[x].resize(obj.size.y);

        for (size_t y = 0; y < obj.size.y; y++) {
            int type;
            sf::Vector2f pos = {(float)x * Tile::tex_size.x, (float)y * Tile::tex_size.y};
            data >> type;
            obj.map[x][y] = obj.generateTileFromId(type, pos, {(unsigned int)x, (unsigned int)y});
            data >> *obj.map[x][y];
        }
    }

    obj.updatePosition();

    return data;
}

std::ofstream &operator<<(std::ofstream &data, const LevelScreen &obj) {
    data << obj.size.x << ' ' << obj.size.y << '\n';

    for (size_t x = 0; x < obj.size.x; x++) {
        for (size_t y = 0; y < obj.size.y; y++) {
            data << *obj.map[x][y] << ' ';
        }
        data << '\n';
    }

    return data;
}

Tile *LevelScreen::generateTileFromId(int id, sf::Vector2f pos, sf::Vector2u tile_pos) {
    switch (id) {
        case Tile::tile_type::AIR:
            return new Tile(this, Tile::tile_type::AIR, pos, tile_pos);
            break;
        case Tile::tile_type::FLOOR:
            return new Tile(this, Tile::tile_type::FLOOR, pos, tile_pos);
            break;
        case Tile::tile_type::WALL:
            return new Tile(this, Tile::tile_type::WALL, pos, tile_pos);
            break;
        case Tile::tile_type::SPAWNER:
            return new SpawnerTile(this, pos, tile_pos);
            break;
        case Tile::tile_type::FINISH:
            return new FinishTile(this, pos, tile_pos);
            break;
        case Tile::tile_type::BONUS:
            return new BonusTile(this, pos, tile_pos);
            break;
        default:
            break;
    }
    throw std::runtime_error("unknown tile type\n");
    return nullptr;
}

void LevelScreen::activateManualControlls() {
    addEventHandler([](sf::Event &event, void *data) {
        LevelScreen *obj = (LevelScreen *)data;

        sf::Keyboard::Key key = event.key.code;

        for (auto &bot : obj->bots) {
            if (key == sf::Keyboard::D)
                bot->rotate(Bot::option::clockwise);

            if (key == sf::Keyboard::A)
                bot->rotate(Bot::option::counterclockwise);

            if (key == sf::Keyboard::W)
                bot->drive(Bot::option::forward);

            if (key == sf::Keyboard::S)
                bot->drive(Bot::option::backward);
        }
        return true;
    },
                    this, 1, sf::Event::KeyPressed);
}

void LevelScreen::setup() {
    top_bar->updatePosition();
}

void LevelScreen::render() {
    clear();
    boni_collected = true;

    for (size_t x = 0; x < this->size.x; x++) {
        for (size_t y = 0; y < this->size.y; y++) {
            map[x][y]->update();
        }
    }

    for (size_t x = 0; x < this->size.x; x++) {
        for (size_t y = 0; y < this->size.y; y++) {
            map[x][y]->render();
        }
    }

    level_completed = win_conditions.size() > 0;
    for (auto &condition : win_conditions) {
        if (!condition->isMet()) {
            level_completed = false;
            break;
        }
    }

    for (auto &bot : bots)
        bot->update();

    for (auto &bot : bots)
        bot->render();

    top_bar->render();

    battery_not_empty = elapsed_time < 10;

    if (level_completed) {
        if (!level_complete_dialog) {
            bool best_so_far = saveState();

            sf::Vector2f button_size = {170, 40};
            sf::Vector2f star_size = {200, 200};
            sf::Vector2f dialog_size = {800, 500};
            float text_size = 40;
            float button_text_size = 20;
            float star_padding = (dialog_size.x - star_size.x * 3) / 4;
            float button_padding = 10;

            sf::Vector2f text_pos = {0, 20};
            sf::Vector2f sub_text_pos = {0, 80};
            sf::Vector2f star_pos = {star_padding, dialog_size.y - star_size.y - button_size.y - star_padding * 2};
            sf::Vector2f button_pos = {dialog_size.x - button_size.x - button_padding, dialog_size.y - button_size.y - button_padding};

            sf::Vector2f dialog_pos = {(view_size.x - dialog_size.x) / 2, (view_size.y - dialog_size.y) / 2};
            level_complete_dialog = new Dialog(this, dialog_pos, dialog_size, 4, GameStyle::DARK_GRAY);

            level_complete_dialog->addText(text_pos, "Level complete!", text_size, GameStyle::GOLD, true);

            level_complete_dialog->addText(sub_text_pos, best_so_far ? "This is your best result!" : "You've done better!", button_text_size, best_so_far ? GameStyle::GREEN : GameStyle::RED, true);

            level_complete_dialog->addImage(star_pos, star_size, &(star_textures[1][0]));
            star_pos.x += star_padding + star_size.x;

            level_complete_dialog->addImage(star_pos, star_size, &(star_textures[battery_not_empty][1]));
            star_pos.x += star_padding + star_size.x;

            level_complete_dialog->addImage(star_pos, star_size, &(star_textures[boni_collected][2]));

            auto *temp = level_complete_dialog->addButton(button_pos, button_size, "Back to menu", button_text_size, 2, GameStyle::BLACK, GameStyle::RED);
            temp->setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, Window::event_close));

            button_pos.x -= button_padding + button_size.x;

            if (next_level_exists) {
                temp = level_complete_dialog->addButton(button_pos, button_size, "Next level", button_text_size, 2, GameStyle::BLACK, GameStyle::GREEN);
                temp->setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, LevelScreen::onNextLevel));
            }
        }

        level_complete_dialog->render();
    }
}

void LevelScreen::updatePosition() {
    for (size_t x = 0; x < this->size.x; x++) {
        for (size_t y = 0; y < this->size.y; y++) {
            map[x][y]->setScale(this->scale);
            map[x][y]->setPosition(this->origin.x + x * (Tile::tex_size.x * this->scale), this->origin.y + y * (Tile::tex_size.y * this->scale));
            map[x][y]->setTilePosition(x, y);
        }
    }

    for (auto &bot : bots)
        bot->updatePosition();
}

bool LevelScreen::onScroll(sf::Event &event, void *data) {
    float zoom_rate = 1.2;

    LevelScreen *obj = (LevelScreen *)data;

    if (obj->level_complete_dialog != nullptr)
        return false;

    float delta = event.mouseWheel.delta > 0 ? event.mouseWheel.delta * zoom_rate : 1 / (-event.mouseWheel.delta * zoom_rate);

    obj->scale *= delta;

    sf::Vector2f mouse_pos_now = {obj->mouse_pos.x - obj->origin.x, obj->mouse_pos.y - obj->origin.y};
    sf::Vector2f mouse_pos_after = {mouse_pos_now.x * delta, mouse_pos_now.y * delta};

    obj->origin.x -= mouse_pos_after.x - mouse_pos_now.x;
    obj->origin.y -= mouse_pos_after.y - mouse_pos_now.y;

    obj->updatePosition();
    return false;
}

bool LevelScreen::onMouseMove(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    if (obj->level_complete_dialog != nullptr)
        return false;

    obj->mouse_pos = {(float)event.mouseMove.x, (float)event.mouseMove.y};

    if (obj->mouse_clicked[sf::Mouse::Button::Left]) {
        sf::Vector2f drag_dist = {event.mouseMove.x - obj->last_mouse_click_pos.x, event.mouseMove.y - obj->last_mouse_click_pos.y};
        obj->origin.x = obj->prev_origin.x + drag_dist.x;
        obj->origin.y = obj->prev_origin.y + drag_dist.y;

        obj->updatePosition();
    }

    return false;
}

bool LevelScreen::onMouseButton(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    if (obj->level_complete_dialog != nullptr)
        return false;

    if (event.mouseButton.button != sf::Mouse::Button::Left)
        return false;

    if (event.type == sf::Event::MouseButtonPressed) {
        obj->last_mouse_click_pos = {(float)event.mouseButton.x, (float)event.mouseButton.y};
        obj->prev_origin = obj->origin;
    }

    return true;
}

bool LevelScreen::onNextLevel(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    obj->next_level_flag = true;
    obj->close();

    return true;
}

void LevelScreen::loadTextures() {
    for (int i = 0; i < 3; i++) {
        star_textures[0][i].loadFromFile("_assets/textures/stars/" + std::to_string(i) + "sw.png");
        star_textures[1][i].loadFromFile("_assets/textures/stars/" + std::to_string(i) + ".png");
    }
    textures_loaded = true;
}

int LevelScreen::checkState(std::string filename, bool &completed, bool &energy, bool &boni) {
    if (std::filesystem::exists(filename)) {
        std::ifstream savefile(filename);

        savefile >> completed;
        savefile >> energy;
        savefile >> boni;
    } else {
        completed = false;
        energy = false;
        boni = false;
    }

    return (int)completed + (int)energy + (int)boni;
}

bool LevelScreen::saveState() {
    std::string save_path = "_user/gamesave/" + this->path;

    std::filesystem::create_directories(save_path);

    bool old_completed, old_energy, old_boni;
    int old_progress = checkState(save_path + "/completion_state.info", old_completed, old_energy, old_boni);
    int progress = (int)level_completed + (int)battery_not_empty + (int)boni_collected;

    if (old_completed && !level_completed) return false;
    if (old_progress > progress) return false;

    std::ofstream savefile(save_path + "/completion_state.info");

    savefile << level_completed << ' ';
    savefile << battery_not_empty << ' ';
    savefile << boni_collected << ' ';

    savefile.close();
    return true;
}