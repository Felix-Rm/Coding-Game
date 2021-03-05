#include "level_screen.h"

LevelScreen::LevelScreen(sf::VideoMode v, std::string title, sf::Uint32 style, int level_num, std::string &path) : Window(v, title, style)
{
    Tile::loadTextures();
    Bot::loadTextures();

    this->top_bar = new TopBar(this);

    this->path = path + std::to_string(level_num) + '/';

    std::ifstream level_info(this->path + "level.info");
    if (!level_info)
        throw std::runtime_error("could not open level info");

    level_info >> this->size.x >> this->size.y;

    int level_height = this->size.y * Tile::tex_size.y;
    int level_width = this->size.x * Tile::tex_size.x;

    this->scale = min(this->view_size.x / level_width, this->view_size.y / level_height);
    this->origin.x = (this->view_size.x - (level_width * this->scale)) / 2;
    this->origin.y = (this->view_size.y - (level_height * this->scale)) / 2;

    for (int y = 0; y < this->size.y; y++)
    {
        this->map.push_back(std::vector<Tile *>());
        auto current_row = &this->map.back();

        for (int x = 0; x < this->size.x; x++)
        {
            int type;
            sf::Vector2f pos = {(float)x * Tile::tex_size.x, (float)y * Tile::tex_size.y};
            level_info >> type;

            switch (type)
            {
            case tile_types::AIR:
                current_row->push_back(new Tile(false, tile_types::AIR, pos));
                break;
            case tile_types::FLOOR:
                current_row->push_back(new Tile(true, tile_types::FLOOR, pos));
                break;
            case tile_types::WALL:
                current_row->push_back(new Tile(false, tile_types::WALL, pos));
                break;

            default:
                printf("unknown tile type\n");
                break;
            }
        }
    }

    level_info >> num_bots;

    for (int i = 0; i < num_bots; i++)
    {
        int x, y;
        level_info >> x >> y;
        bots.push_back(new Bot({(float)x * Tile::tex_size.y, (float)y * Tile::tex_size.y}, {(float)x, (float)y}, level_info, this));
    }

    updatePosition();

    addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    addEventHandler(onScroll, this, 1, sf::Event::MouseWheelMoved);
    addEventHandler(onMouseButton, this, 2, sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased);

    level_info.close();

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

void LevelScreen::setup() {}

void LevelScreen::render()
{
    clear();

    for (int y = 0; y < this->size.y; y++)
    {
        for (int x = 0; x < this->size.x; x++)
        {
            map[y][x]->render(this);
        }
    }

    for (int i = 0; i < num_bots; i++)
        bots[i]->update();

    for (int i = 0; i < num_bots; i++)
        bots[i]->render(this);

    top_bar->render(this);
}

void LevelScreen::updatePosition()
{

    for (int y = 0; y < this->size.y; y++)
    {
        for (int x = 0; x < this->size.x; x++)
        {
            map[y][x]->setScale(this->scale);
            map[y][x]->setPosition(this->origin.x + x * (Tile::tex_size.x * this->scale), this->origin.y + y * (Tile::tex_size.y * this->scale));
        }
    }

    for (int i = 0; i < num_bots; i++)
    {
        bots[i]->updatePosition();
    }
}

bool LevelScreen::onScroll(sf::Event &event, void *data)
{
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

bool LevelScreen::onMouseMove(sf::Event &event, void *data)
{
    LevelScreen *obj = (LevelScreen *)data;

    obj->mouse_pos = {(float)event.mouseMove.x, (float)event.mouseMove.y};

    if (obj->mouse_down)
    {

        sf::Vector2f drag_dist = {event.mouseMove.x - obj->last_mouse_click_pos.x, event.mouseMove.y - obj->last_mouse_click_pos.y};
        obj->origin.x = obj->prev_origin.x + drag_dist.x;
        obj->origin.y = obj->prev_origin.y + drag_dist.y;

        obj->updatePosition();
    }

    return true;
}

bool LevelScreen::onMouseButton(sf::Event &event, void *data)
{
    if (event.mouseButton.button != 0)
        return false;

    LevelScreen *obj = (LevelScreen *)data;

    if (event.type == sf::Event::MouseButtonPressed)
    {
        obj->mouse_down = true;
        obj->last_mouse_click_pos = {(float)event.mouseButton.x, (float)event.mouseButton.y};
        obj->prev_origin = obj->origin;
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        obj->mouse_down = false;
    }
    return true;
}