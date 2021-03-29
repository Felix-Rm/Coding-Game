#include "level_screen.h"

LevelScreen::LevelScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string &path) : Window(video_mode, title, style) {
    Tile::loadTextures();
    Bot::loadTextures();

    this->top_bar = new TopBar(this);

    this->path = path + std::to_string(level_num) + '/';

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

    addEventHandler([](sf::Event &event, void *data) {
        LevelScreen *obj = (LevelScreen *)data;

        sf::Keyboard::Key key = event.key.code;

        if (key == sf::Keyboard::D)
            obj->bots[0]->rotate(Bot::option::clockwise);

        if (key == sf::Keyboard::A)
            obj->bots[0]->rotate(Bot::option::counterclockwise);

        if (key == sf::Keyboard::W)
            obj->bots[0]->drive(Bot::option::forward);

        if (key == sf::Keyboard::S)
            obj->bots[0]->drive(Bot::option::backward);

        return true;
    },
                    this, 1, sf::Event::KeyPressed);
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
            obj.map[x][y] = obj.generateTileFromId(type, pos);
            data >> *obj.map[x][y];
        }
    }

    obj.updatePosition();

    return data;
}

std::ofstream &operator<<(std::ofstream &data, const LevelScreen &obj) {
    data << obj.size.x << obj.size.y;

    for (size_t x = 0; x < obj.size.x; x++) {
        for (size_t y = 0; y < obj.size.y; y++) {
            data << *obj.map[x][y];
        }
    }

    return data;
}

Tile *LevelScreen::generateTileFromId(int id, sf::Vector2f pos) {
    switch (id) {
        case Tile::tile_type::AIR:
            return new Tile(this, Tile::tile_type::AIR, pos);
            break;
        case Tile::tile_type::FLOOR:
            return new Tile(this, Tile::tile_type::FLOOR, pos);
            break;
        case Tile::tile_type::WALL:
            return new Tile(this, Tile::tile_type::WALL, pos);
            break;
        case Tile::tile_type::SPAWNER:
            return new SpawnerTile(this, pos);
            break;
        default:
            break;
    }
    throw std::runtime_error("unknown tile type\n");
    return nullptr;
}

void LevelScreen::setup() {
    top_bar->updatePosition();
}

void LevelScreen::render() {
    clear();

    for (size_t x = 0; x < this->size.x; x++) {
        for (size_t y = 0; y < this->size.y; y++) {
            map[x][y]->render();
        }
    }

    for (int i = 0; i < num_bots; i++)
        bots[i]->update();

    for (int i = 0; i < num_bots; i++)
        bots[i]->render();

    top_bar->render();
}

void LevelScreen::updatePosition() {
    for (size_t x = 0; x < this->size.x; x++) {
        for (size_t y = 0; y < this->size.y; y++) {
            map[x][y]->setScale(this->scale);
            map[x][y]->setPosition(this->origin.x + x * (Tile::tex_size.x * this->scale), this->origin.y + y * (Tile::tex_size.y * this->scale));
        }
    }

    for (int i = 0; i < num_bots; i++) {
        bots[i]->updatePosition();
    }
}

bool LevelScreen::onScroll(sf::Event &event, void *data) {
    float zoom_rate = 1.2;

    LevelScreen *obj = (LevelScreen *)data;
    float delta = event.mouseWheel.delta > 0 ? event.mouseWheel.delta * zoom_rate : 1 / (-event.mouseWheel.delta * zoom_rate);

    obj->scale *= delta;

    sf::Vector2f mouse_pos_now = {obj->mouse_pos.x - obj->origin.x, obj->mouse_pos.y - obj->origin.y};
    sf::Vector2f mouse_pos_after = {mouse_pos_now.x * delta, mouse_pos_now.y * delta};

    obj->origin.x -= mouse_pos_after.x - mouse_pos_now.x;
    obj->origin.y -= mouse_pos_after.y - mouse_pos_now.y;

    obj->updatePosition();
    return true;
}

bool LevelScreen::onMouseMove(sf::Event &event, void *data) {
    LevelScreen *obj = (LevelScreen *)data;

    obj->mouse_pos = {(float)event.mouseMove.x, (float)event.mouseMove.y};

    if (obj->mouse_down) {
        sf::Vector2f drag_dist = {event.mouseMove.x - obj->last_mouse_click_pos.x, event.mouseMove.y - obj->last_mouse_click_pos.y};
        obj->origin.x = obj->prev_origin.x + drag_dist.x;
        obj->origin.y = obj->prev_origin.y + drag_dist.y;

        obj->updatePosition();
    }

    return true;
}

bool LevelScreen::onMouseButton(sf::Event &event, void *data) {
    if (event.mouseButton.button != 0)
        return false;

    LevelScreen *obj = (LevelScreen *)data;

    if (event.type == sf::Event::MouseButtonPressed) {
        obj->mouse_down = true;
        obj->last_mouse_click_pos = {(float)event.mouseButton.x, (float)event.mouseButton.y};
        obj->prev_origin = obj->origin;
    } else if (event.type == sf::Event::MouseButtonReleased) {
        obj->mouse_down = false;
    }
    return true;
}