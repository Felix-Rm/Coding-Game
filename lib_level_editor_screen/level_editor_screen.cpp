#include "level_editor_screen.h"

LevelEditorScreen::LevelEditorScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string& load_path, std::string& save_path)
    : LevelScreen(video_mode, title, style, level_num, load_path, save_path) {
    this->frame = sf::RectangleShape({0, 0});
    this->frame.setFillColor(sf::Color::Transparent);
    this->frame.setOutlineThickness(5);
    this->frame.setOutlineColor(GameStyle::GOLD);

    this->cursor_outline = sf::RectangleShape({0, 0});
    this->cursor_outline.setFillColor(sf::Color::Transparent);
    this->cursor_outline.setOutlineThickness(2);
    this->cursor_outline.setOutlineColor(GameStyle::GOLD);

    this->cursor = sf::Sprite(Tile::textures[this->tile_type]);

    addEventHandler(onMouseMove, this, 2, sf::Event::MouseMoved, sf::Event::MouseWheelMoved);
    addEventHandler(onMouseButton, this, 2, sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased);
    addEventHandler(onKeyEvent, this, 1, sf::Event::KeyPressed);

    updateOverlay();
}

bool LevelEditorScreen::onMouseButton(sf::Event& event, void* data) {
    LevelEditorScreen* obj = (LevelEditorScreen*)data;

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == 1) {
        obj->save();
        return true;
    }

    if (obj->mouse_clicked[sf::Mouse::Button::Right])
        return obj->placeTileAtMousePos();

    return false;
}

bool LevelEditorScreen::onMouseMove(sf::Event& event, void* data) {
    LevelEditorScreen* obj = (LevelEditorScreen*)data;

    if (obj->mouse_clicked[sf::Mouse::Button::Right])
        obj->placeTileAtMousePos();

    if (event.type == sf::Event::MouseWheelMoved || event.type == sf::Event::MouseMoved)
        obj->updateOverlay();
    return true;
}

bool LevelEditorScreen::onKeyEvent(sf::Event& event, void* data) {
    LevelEditorScreen* obj = (LevelEditorScreen*)data;

    sf::Keyboard::Key key = event.key.code;
    bool modified_map = false;

    if (key == sf::Keyboard::W && obj->tile_type < Tile::tile_type::_COUNT - 1) {
        obj->tile_type++;
    } else if (key == sf::Keyboard::S && obj->tile_type > 0) {
        obj->tile_type--;
    } else if (key == sf::Keyboard::I) {
        modified_map = true;
        if (event.key.shift) {
            if (obj->map[0].size() <= 1) return false;
            obj->size.y--;
            for (size_t x = 0; x < obj->size.x; x++) {
                delete obj->map[x][0];
                obj->map[x].erase(obj->map[x].begin());
            }
        } else {
            obj->size.y++;
            for (size_t x = 0; x < obj->size.x; x++) {
                obj->map[x].insert(obj->map[x].begin(), new Tile(obj, Tile::tile_type::AIR, {0, 0}, {0, 0}));
            }
        }
    } else if (key == sf::Keyboard::K) {
        modified_map = true;
        if (event.key.shift) {
            if (obj->map[0].size() <= 1) return false;
            obj->size.y--;
            for (size_t x = 0; x < obj->size.x; x++) {
                delete obj->map[x][obj->size.y];
                obj->map[x].pop_back();
            }

        } else {
            obj->size.y++;
            for (size_t x = 0; x < obj->size.x; x++) {
                obj->map[x].insert(obj->map[x].end(), new Tile(obj, Tile::tile_type::AIR, {0, 0}, {0, 0}));
            }
        }
    } else if (key == sf::Keyboard::J) {
        modified_map = true;
        if (event.key.shift) {
            if (obj->map.size() <= 1) return false;
            obj->size.x--;
            for (size_t y = 0; y < obj->size.y; y++) {
                delete obj->map[0][y];
            }
            obj->map.erase(obj->map.begin());

        } else {
            obj->size.x++;
            obj->map.insert(obj->map.begin(), std::vector<Tile*>());
            for (size_t y = 0; y < obj->size.y; y++) {
                obj->map[0].push_back(new Tile(obj, Tile::tile_type::AIR, {0, 0}, {0, 0}));
            }
        }
    } else if (key == sf::Keyboard::L) {
        modified_map = true;
        if (event.key.shift) {
            if (obj->map.size() <= 1) return false;
            obj->size.x--;
            for (size_t y = 0; y < obj->size.y; y++) {
                delete obj->map[obj->size.x][y];
            }
            obj->map.pop_back();

        } else {
            obj->map.push_back(std::vector<Tile*>());
            for (size_t y = 0; y < obj->size.y; y++) {
                obj->map[obj->size.x].push_back(new Tile(obj, Tile::tile_type::AIR, {0, 0}, {0, 0}));
            }
            obj->size.x++;
        }
    }

    obj->cursor = sf::Sprite(Tile::textures[obj->tile_type]);
    obj->updateOverlay();

    if (modified_map) {
        obj->updatePosition();
        obj->save();
    }
    return true;
}

void LevelEditorScreen::save() {
    std::ofstream file(this->load_path + "level.info");
    file << *this;
    file.close();
}

bool LevelEditorScreen::placeTileAtMousePos() {
    unsigned int tile_pos_x = (this->mouse_pos.x - this->origin.x) / (Tile::tex_size.x * this->scale);
    unsigned int tile_pos_y = (this->mouse_pos.y - this->origin.y) / (Tile::tex_size.y * this->scale);

    if (last_tile_placement.tile_pos_x == tile_pos_x && last_tile_placement.tile_pos_y == tile_pos_y && last_tile_placement.tile_type == tile_type)
        return false;

    if (tile_pos_x < 0 || tile_pos_x >= this->size.x || tile_pos_y < 0 || tile_pos_y >= this->size.y)
        return false;

    delete this->map[tile_pos_x][tile_pos_y];
    this->map[tile_pos_x][tile_pos_y] = this->generateTileFromId(this->tile_type, {0, 0}, {tile_pos_x, tile_pos_y});
    this->updatePosition();

    last_tile_placement.tile_pos_x = tile_pos_x;
    last_tile_placement.tile_pos_y = tile_pos_y;
    last_tile_placement.tile_type = tile_type;

    return true;
}

void LevelEditorScreen::updateOverlay() {
    this->frame.setPosition(this->origin);
    this->frame.setSize({this->size.x * Tile::tex_size.x * this->scale, this->size.y * Tile::tex_size.y * this->scale});

    this->cursor.setPosition(this->mouse_pos);
    this->cursor.setScale(this->scale * 0.5f, this->scale * 0.5f);

    this->cursor_outline.setPosition(this->mouse_pos);
    this->cursor_outline.setSize({Tile::tex_size.x * this->scale * 0.5f, Tile::tex_size.y * this->scale * 0.5f});
}

void LevelEditorScreen::setup() {
    updateOverlay();
}

void LevelEditorScreen::render() {
    LevelScreen::render();
    draw(frame);
    draw(cursor);
    draw(cursor_outline);
}