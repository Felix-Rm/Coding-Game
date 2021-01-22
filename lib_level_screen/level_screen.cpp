#include "level_screen.h"

LevelScreen::LevelScreen(sf::VideoMode v, std::string title, sf::Uint32 style, int level_num, std::string &path) : Window(v, title, style)
{
    this->path = path + std::to_string(level_num) + '/';

    std::ifstream level_info(this->path + "level.info");
    level_info >> this->size.x >> this->size.y;

    for (int y = 0; y < this->size.y; y++)
    {
        this->map.push_back(std::vector<Tile *>());
        auto current_row = &this->map.back();

        for (int x = 0; x < this->size.x; x++)
        {
            int type;
            level_info >> type;

            switch (type)
            {
            case tile_types::AIR:
                current_row->push_back(new Tile(false, tile_types::AIR));
                break;
            case tile_types::FLOOR:
                current_row->push_back(new Tile(true, tile_types::FLOOR));
                break;
            case tile_types::WALL:
                current_row->push_back(new Tile(false, tile_types::WALL));
                break;

            default:
                printf("unknown tile type\n");
                break;
            }
        }
    }

    level_info.close();
}

void LevelScreen::render()
{
    for (int y = 0; y < this->size.y; y++)
    {

        for (int x = 0; x < this->size.x; x++)
        {
            map[y][x]->render(this);
        }
    }
}