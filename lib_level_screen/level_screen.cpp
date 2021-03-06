#include "level_screen.h"

sf::Texture LevelScreen::star_textures[2][3];
bool LevelScreen::textures_loaded = false;

LevelScreen::LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &load_path, std::string &save_path) : Window(video_mode, title, style) {
    if (!textures_loaded)
        loadTextures();

    Tile::loadTextures();
    Bot::loadTextures();

     top_bar = new TopBar(this);

    this->load_path = load_path + std::to_string(level_num) + '/';
    this->save_path = save_path + std::to_string(level_num) + '/';

    if (!std::filesystem::exists(this->load_path + "level.info")) {
        printf("creating not existing level info\n");
        std::filesystem::create_directories(this->load_path);
        std::filesystem::copy_file("_assets/presets/default_level.info", this->load_path + "level.info");
    }

    if (!std::filesystem::exists(this->save_path + "code/")) {
        printf("creating not existing level info\n");
        std::filesystem::create_directories(this->save_path + "code/");
        std::filesystem::copy("_assets/presets/default_code/", this->save_path + "code/");
    }

    load();

    addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    addEventHandler(onScroll, this, 1, sf::Event::MouseWheelMoved);
    addEventHandler(onMouseButton, this, 2, sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased);
}

void LevelScreen::load() {
    elapsed_time = 0;
    top_bar->update();

    for (auto &row : map) {
        for (auto &cell : row)
            delete cell;
        row.clear();
    }
    map.clear();

    std::ifstream level_info(this->load_path + "level.info");
    if (!level_info)
        throw std::runtime_error("could not open level info");

    level_info >> *this;

    level_info.close();
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

    for (size_t i = 0; i < obj.bots.size(); i++) {
        std::string code_filename = obj.save_path + "code/robot_0x" + std::to_string(i) + ".cs";
        if (!std::filesystem::exists(code_filename)) {
            std::filesystem::copy_file("_assets/presets/robot.cs", code_filename);
            std::string cmd = "sed -i s/botnum/" + std::to_string(i) + "/g " + code_filename;
            system(cmd.c_str());
        }
    }

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
    if (running) {
        for (size_t i = 0; i < bots.size(); i++) {
            std::string folder = "_assets/communication/bot" + std::to_string(i) + '/';

            if (std::filesystem::exists(folder + "drive_forward")) {
                bots[i]->drive(Bot::forward);
                bool b = std::filesystem::remove(folder + "drive_forward");
                printf("driving forward %d\n", b);
            } else if (std::filesystem::exists(folder + "drive_backward")) {
                printf("driving backward\n");
                bots[i]->drive(Bot::backward);
                std::filesystem::remove(folder + "drive_backward");
            } else if (std::filesystem::exists(folder + "rotate_ccw")) {
                printf("rotating counterclockwise\n");
                bots[i]->rotate(Bot::counterclockwise);
                std::filesystem::remove(folder + "rotate_ccw");
            } else if (std::filesystem::exists(folder + "rotate_cw")) {
                printf("rotating clockwise\n");
                bots[i]->rotate(Bot::clockwise);
                std::filesystem::remove(folder + "rotate_cw");
            }

            if (bots[i]->is_just_done()) {
                std::ofstream ofs(folder + "done");
                ofs.close();
            }
        }
    }

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

bool LevelScreen::onPlay(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    obj->running = !obj->running;

    if (obj->running) {
        std::string cmd = "bash _assets/presets/run_code.sh " + obj->save_path + "code/";
        system(cmd.c_str());
        obj->top_bar->btn_stop_play.setImage(GameStyle::Icon::STOP);
    } else {
        obj->load();
        obj->top_bar->btn_stop_play.setImage(GameStyle::Icon::PLAY);
    }

    return true;
}

bool LevelScreen::onEditor(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    std::string command = "code " + obj->save_path + "code/ &";
    system(command.c_str());

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