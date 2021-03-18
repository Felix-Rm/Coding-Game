#include "bot.h"

std::string Bot::path = "assets/textures/bots/";
bool Bot::textures_loaded = false;
sf::Texture Bot::textures[Bot::num_textures];
sf::Vector2f Bot::delta_tile_position_per_update = {1.0 / Bot::updates_per_movement, 1.0 / Bot::updates_per_movement};

Bot::Bot(Window *window, sf::Vector2f pos, sf::Vector2f tile_pos, std::ifstream &data, LevelScreen *lvl) : Drawable(window, pos, Tile::tex_size) {
    if (!textures_loaded)
        throw std::runtime_error("Textures have to be loaded before Bot can be constructed!");

    this->applyOffset();

    this->body = sf::Sprite(textures[0]);
    this->body.setOrigin(Tile::tex_size.x / 2, Tile::tex_size.y / 2);
    this->body.setPosition(this->pos);

    this->tile_position = tile_pos;
    this->target_position = tile_pos;

    this->level = lvl;
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
    if (movement_complete == MOVING)
        movement_complete = JUST_DONE;

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

        movement_complete = MOVING;
    }

    float delta_x = target_position.x - tile_position.x;
    if (abs(delta_x) > delta_tile_position_per_update.x / 2) {
        tile_position.x += delta_x > 0 ? delta_tile_position_per_update.x : -delta_tile_position_per_update.x;
        movement_complete = MOVING;
    }

    float delta_y = target_position.y - tile_position.y;
    if (abs(delta_y) > delta_tile_position_per_update.y / 2) {
        tile_position.y += delta_y > 0 ? delta_tile_position_per_update.y : -delta_tile_position_per_update.y;
        movement_complete = MOVING;
    }

    if (movement_complete == MOVING) {
        updatePosition();
        this->body.setRotation(current_rotation);
    }

    if (movement_complete == JUST_DONE) {
        current_rotation = target_rotation;

        tile_position.x = target_position.x;
        tile_position.y = target_position.y;

        current_rotation = target_rotation;

        updatePosition();
        this->body.setRotation(current_rotation);

        level->top_bar->update();
        movement_complete = DONE;
    }
}

bool Bot::rotate(option dir) {
    if (movement_complete != DONE)
        return false;

    if (dir == clockwise)
        target_rotation += 45;
    if (dir == counterclockwise)
        target_rotation -= 45;

    movement_complete = MOVING;
    level->elapsed_time += 0.5;
    return true;
}

bool Bot::drive(option dir) {
    if (movement_complete != DONE)
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

    if (dir == forward) {
        target_position.x += delta_pos.x;
        target_position.y += delta_pos.y;
    }

    if (dir == backward) {
        target_position.x -= delta_pos.x;
        target_position.y -= delta_pos.y;
    }

    movement_complete = MOVING;
    level->elapsed_time += (int)current_rotation % 90 == 0 ? 1 : 1.5;
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