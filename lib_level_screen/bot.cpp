#include "bot.h"

std::string Bot::path = "_assets/textures/bots/";
bool Bot::textures_loaded = false;
sf::Texture Bot::textures[Bot::num_textures];
sf::Vector2f Bot::delta_tile_position_per_update = {1.0 / Bot::updates_per_movement, 1.0 / Bot::updates_per_movement};

Bot::Bot(LevelScreen *window, sf::Vector2f pos, sf::Vector2u tile_pos) : Drawable((Window *)window, pos, Tile::tex_size) {
    if (!textures_loaded)
        throw std::runtime_error("Textures have to be loaded before Bot can be constructed!");

    this->applyOffset();

    this->body = sf::Sprite(textures[0]);
    this->body.setOrigin(Tile::tex_size.x / 2, Tile::tex_size.y / 2);
    this->body.setPosition(this->pos);

    this->tile_position = {(float)tile_pos.x, (float)tile_pos.y};
    this->target_position = {(float)tile_pos.x, (float)tile_pos.y};

    this->level = window;
}

void Bot::updatePosition() {
    setScale(level->scale);
    setPosition(level->origin.x + (tile_position.x * Tile::tex_size.x * level->scale), level->origin.y + (tile_position.y * Tile::tex_size.y * level->scale));
}

void Bot::setPosition(float x, float y) {
    this->pos = {x, y};
    this->applyOffset();

    this->body.setPosition(this->pos);
};

void Bot::setTilePosition(unsigned int x, unsigned int y) {
    this->tile_position = {x, y};
    this->target_position = {x, y};

    this->updatePosition();
}

void Bot::shiftPosition(float dx, float dy) {
    this->pos.x += dx;
    this->pos.y += dy;

    this->body.setPosition(this->pos);
};

void Bot::applyOffset() {
    auto scale = this->body.getScale();
    this->pos.x += Tile::tex_size.x / 2 * scale.x;
    this->pos.y += Tile::tex_size.y / 2 * scale.y;
}

void Bot::setScale(float s) {
    this->body.setScale(s, s);
}

void Bot::render() {
    this->window->draw(this->body);
}

void Bot::update() {
    int repeats = window->mspf / time_for_movement * updates_per_movement;

    if (state == MOVING) {
        t += repeats;
    }

    for (int i = 0; i < repeats && state != IDLE; i++) {
        if (state == MOVING)
            state = JUST_DONE;

        if (abs(current_rotation - target_rotation) > delta_rotation_per_update) {
            float_mod(target_rotation, 360);
            float_mod(current_rotation, 360);

            float dir1 = target_rotation - current_rotation;
            float dir2 = (target_rotation + 360) - current_rotation;
            float dir3 = -(current_rotation + 360) + target_rotation;

            float chosen_dir = 0;
            if (abs(dir1) < abs(dir2) && abs(dir1) < abs(dir3)) {
                chosen_dir = dir1;
                //printf("#1: ");
            } else if (abs(dir2) < abs(dir3)) {
                chosen_dir = dir2;
                //printf("#2: ");
            } else {
                chosen_dir = dir3;
                //printf("#3: ");
            }

            //printf("current: %f target: %f move: %f\n", current_rotation, target_rotation, chosen_dir);
            current_rotation += chosen_dir > 0 ? delta_rotation_per_update : -delta_rotation_per_update;

            state = MOVING;
        }

        float delta_x = target_position.x - tile_position.x;
        if (abs(delta_x) > delta_tile_position_per_update.x / 2) {
            tile_position.x += delta_x > 0 ? delta_tile_position_per_update.x : -delta_tile_position_per_update.x;
            state = MOVING;
        }

        float delta_y = target_position.y - tile_position.y;
        if (abs(delta_y) > delta_tile_position_per_update.y / 2) {
            tile_position.y += delta_y > 0 ? delta_tile_position_per_update.y : -delta_tile_position_per_update.y;
            state = MOVING;
        }

        if (state == MOVING) {
            updatePosition();
            this->body.setRotation(current_rotation);
        }

        if (state == JUST_DONE) {
            current_rotation = target_rotation;

            tile_position.x = target_position.x;
            tile_position.y = target_position.y;

            current_rotation = target_rotation;

            updatePosition();
            this->body.setRotation(current_rotation);

            level->top_bar->update();
            state = IDLE;
        }
    }
}

bool Bot::rotate(option dir) {
    if (state != IDLE)
        return false;

    if (dir == clockwise)
        target_rotation += 45;
    if (dir == counterclockwise)
        target_rotation -= 45;

    state = MOVING;

    time_for_movement = 0.5 * ((LevelScreen *)window)->ms_per_unit_movement;
    level->elapsed_time += 0.5;

    return true;
}

bool Bot::drive(option dir) {
    if (state != IDLE)
        return false;

    sf::Vector2i delta_pos;

    switch ((int)target_rotation) {
        case 0:
            delta_pos = {0, -1};
            break;
        case 45:
        case -315:
            delta_pos = {1, -1};
            break;
        case 90:
        case -270:
            delta_pos = {1, 0};
            break;
        case 135:
        case -225:
            delta_pos = {1, 1};
            break;
        case 180:
        case -180:
            delta_pos = {0, 1};
            break;
        case 225:
        case -135:
            delta_pos = {-1, 1};
            break;
        case 270:
        case -90:
            delta_pos = {-1, 0};
            break;
        case 315:
        case -45:
            delta_pos = {-1, -1};
            break;
        default:
            throw std::runtime_error("You seem to be at a rotation angle which should not be reachable!");
            break;
    }

    if (dir == backward) {
        delta_pos.x *= -1;
        delta_pos.y *= -1;
    }

    target_position.x += delta_pos.x;
    target_position.y += delta_pos.y;

    bool target_ok = target_position.x >= 0 && target_position.y >= 0 && target_position.x <= level->size.x - 1 && target_position.y <= level->size.y - 1;
    if (target_ok) {
        target_ok &= level->map[target_position.x][target_position.y]->isDrivable();
    }

    if (!target_ok) {
        target_position.x -= delta_pos.x;
        target_position.y -= delta_pos.y;
        return false;
    }

    state = MOVING;

    float length = (int)current_rotation % 90 == 0 ? 1 : 1.5;
    time_for_movement = length * ((LevelScreen *)window)->ms_per_unit_movement;
    level->elapsed_time += length;

    return true;
}

void Bot::loadTextures() {
    if (textures_loaded) {
        return;
    }

    for (int i = 0; i < num_textures; i++) {
        textures[i].loadFromFile(path + std::to_string(i) + ".png");
    }

    textures_loaded = true;
}