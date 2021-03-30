#include "level_editor_screen.h"

LevelEditorScreen::LevelEditorScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string& path)
    : LevelScreen(video_mode, title, style, level_num, path) {
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
        std::ofstream file(obj->path + "level.info");
        file << *obj;
        file.close();
        return true;
    }

    if (obj->mouse_down[1])
        return obj->placeTileAtMousePos();

    return false;
}

bool LevelEditorScreen::onMouseMove(sf::Event& event, void* data) {
    LevelEditorScreen* obj = (LevelEditorScreen*)data;

    if (obj->mouse_down[1])
        obj->placeTileAtMousePos();

    if (event.type == sf::Event::MouseWheelScrolled || event.type == sf::Event::MouseMoved)
        obj->updateOverlay();
    return true;
}

bool LevelEditorScreen::onKeyEvent(sf::Event& event, void* data) {
    LevelEditorScreen* obj = (LevelEditorScreen*)data;

    sf::Keyboard::Key key = event.key.code;

    if (key == sf::Keyboard::W && obj->tile_type < Tile::tile_type::_COUNT - 1)
        obj->tile_type++;
    if (key == sf::Keyboard::S && obj->tile_type > 0)
        obj->tile_type--;

    obj->cursor = sf::Sprite(Tile::textures[obj->tile_type]);
    obj->updateOverlay();
    return true;
}

bool LevelEditorScreen::placeTileAtMousePos() {
    unsigned int tile_pos_x = (this->mouse_pos.x - this->origin.x) / (Tile::tex_size.x * this->scale);
    unsigned int tile_pos_y = (this->mouse_pos.y - this->origin.y) / (Tile::tex_size.y * this->scale);

    if (last_tile_placement.tile_pos_x == tile_pos_x && last_tile_placement.tile_pos_y == tile_pos_y && last_tile_placement.tile_type == tile_type)
        return false;

    if (tile_pos_x < 0 || tile_pos_x >= this->size.x || tile_pos_y < 0 || tile_pos_y >= this->size.y)
        return false;

    delete this->map[tile_pos_x][tile_pos_y];
    this->map[tile_pos_x][tile_pos_y] = this->generateTileFromId(this->tile_type, {0, 0});
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