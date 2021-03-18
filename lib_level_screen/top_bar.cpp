#include "top_bar.h"

TopBar::TopBar(LevelScreen *parent_window) : Drawable(nullptr, {}, {}) {
    this->window = parent_window;
    this->background_tex.loadFromFile("assets/textures/miscellaneous/top_bar.png");
    this->background.setTexture(this->background_tex);

    this->size = {this->background.getGlobalBounds().width, this->background.getGlobalBounds().height};

    this->text_size = this->size.y * 0.8;

    updatePosition();
}

void TopBar::updatePosition() {
    this->pos = {(this->window->view_size.x - this->size.x) / 2, 0};
    this->background.setPosition(this->pos);

    this->time_text = sf::Text("00:00s", GameStyle::game_font, text_size);
    this->time_text.setFillColor(GameStyle::GOLD);
    this->time_text.setStyle(sf::Text::Bold);

    auto temp_bounds = this->time_text.getLocalBounds();

    this->time_text.setPosition(
        this->pos.x + (this->size.x * (3 / 9.0) - temp_bounds.width) / 2 - temp_bounds.left,
        this->pos.y + (this->size.y - temp_bounds.height) / 2 - temp_bounds.top);

    this->speed_text = sf::Text("100%", GameStyle::game_font, text_size);
    this->speed_text.setFillColor(GameStyle::GOLD);
    this->speed_text.setStyle(sf::Text::Bold);

    //

    this->btn_play = Button(this->window,
                            {(float)(this->pos.x + this->size.x * (3.5 / 9.0) + this->size.x * (1 / 9)), (float)(this->pos.y + this->size.y * 0.5)},
                            {(float)(this->size.y * 0.8), (float)(this->size.y * 0.8)},
                            "p", this->text_size * 0.8, 1, GameStyle::WHITE, GameStyle::GOLD, Window::createEventHandler(Window::noop, nullptr));
    this->btn_play.center();

    this->btn_stop = Button(this->window,
                            {(float)(this->pos.x + this->size.x * (4.5 / 9.0) + this->size.x * (1 / 9)), (float)(this->pos.y + this->size.y * 0.5)},
                            {(float)(this->size.y * 0.8), (float)(this->size.y * 0.8)},
                            "s", this->text_size * 0.8, 1, GameStyle::WHITE, GameStyle::GOLD, Window::createEventHandler(Window::noop, nullptr));
    this->btn_stop.center();

    this->btn_exit = Button(this->window,
                            {(float)(this->pos.x + this->size.x * (5.5 / 9.0) + this->size.x * (1 / 9)), (float)(this->pos.y + this->size.y * 0.5)},
                            {(float)(this->size.y * 0.8), (float)(this->size.y * 0.8)},
                            "e", this->text_size * 0.8, 1, GameStyle::WHITE, GameStyle::GOLD, Window::createEventHandler([](sf::Event &event, void *data) {
                                ((Window *)data)->close();
                                return true;
                            },
                                                                                                                         this->window));
    this->btn_exit.center();

    //

    temp_bounds = this->speed_text.getLocalBounds();

    this->speed_text.setPosition(
        this->pos.x + this->size.x * (6 / 9.0) + (this->size.x * (2 / 9.0) - temp_bounds.width) / 2 - temp_bounds.left,
        this->pos.y + (this->size.y - temp_bounds.height) / 2 - temp_bounds.top);

    this->btn_speed_plus = Button(this->window,
                                  {(float)(this->pos.x + this->size.x * (6 / 9.0) + this->size.x * (2.5 / 9)), (float)(this->pos.y + this->size.y * (1 / 4.0))},
                                  {(float)(this->size.y * (1 / 3.0)), (float)(this->size.y * (1 / 3.0))},
                                  "+", this->text_size * (1 / 3.0), 1, GameStyle::WHITE, GameStyle::GOLD, Window::createEventHandler(Window::noop, nullptr));
    this->btn_speed_plus.center();

    this->btn_speed_minus = Button(this->window,
                                   {(float)(this->pos.x + this->size.x * (6 / 9.0) + this->size.x * (2.5 / 9)), (float)(this->pos.y + this->size.y * (3 / 4.0))},
                                   {(float)(this->size.y * (1 / 3.0)), (float)(this->size.y * (1 / 3.0))},
                                   "-", this->text_size * (1 / 3.0), 1, GameStyle::WHITE, GameStyle::GOLD, Window::createEventHandler(Window::noop, nullptr));
    this->btn_speed_minus.center();
}

void TopBar::setPosition(float x, float y) {}
void TopBar::shiftPosition(float dx, float dy) {}

void TopBar::render() {
    this->window->draw(this->background);
    this->window->draw(this->speed_text);
    this->window->draw(this->time_text);

    btn_play.render();
    btn_stop.render();
    btn_exit.render();

    btn_speed_plus.render();
    btn_speed_minus.render();
}

void TopBar::update() {
    int time_part_minutes = this->window->elapsed_time / 60;
    int time_part_seconds = (int)this->window->elapsed_time % 60;
    int time_part_milliseconds = (int)(this->window->elapsed_time * 10) % 10;

    char buffer[15] = {' '};
    sprintf(buffer, "%02d:%02d:%01ds", time_part_minutes, time_part_seconds, time_part_milliseconds);

    this->time_text.setString(std::string(buffer));
}
