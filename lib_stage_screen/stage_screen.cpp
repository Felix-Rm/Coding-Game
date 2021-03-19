#include "stage_screen.h"

StageScreen::StageScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : Window(video_mode, title, style) {
    std::ifstream stage_info("assets/stages.info");
    if (!stage_info)
        throw std::runtime_error("could not open stage info");

    stage_info >> this->num_stages >> this->original_stage_size.x >> this->original_stage_size.y;
    stage_info.close();

    for (int i = 0; i < num_stages; i++)
        stage_loaded.push_back(false);

    this->btn_exit = ImageButton(this, {0, 0}, {60, 60},
                                 GameStyle::Icon::EXIT, 0.7, 1, GameStyle::WHITE, GameStyle::RED, Window::createEventHandler([](sf::Event &event, void *data) {
                                     ((Window *)data)->close();
                                     return true;
                                 },
                                                                                                                             this));

    setup();

    addEventHandler(onMouseScroll, this, 1, sf::Event::EventType::MouseWheelScrolled);
}

void StageScreen::calculateStagePositions() {
    constrainScroll();

    int bottom_stage_id = (y_scroll / stage_size.y) - 1;
    int top_stage_id = ((y_scroll + view_size.y) / stage_size.y) + 1;
    int stage_scroll_offset = view_size.y - stage_size.y + (y_scroll);

    printf("bot_id: %d; top_id: %d; offset: %d\n", bottom_stage_id, top_stage_id, stage_scroll_offset);

    if (bottom_stage_id < 0)
        bottom_stage_id = 0;
    if (bottom_stage_id >= num_stages)
        bottom_stage_id = num_stages - 1;
    if (top_stage_id < 0)
        top_stage_id = 0;
    if (top_stage_id >= num_stages)
        top_stage_id = num_stages - 1;

    for (size_t i = 0; i < stage_loaded.size(); i++) {
        if (stage_loaded[i] && ((int)i < bottom_stage_id || (int)i > top_stage_id)) {
            delete stages[i];
            stage_loaded[i] = false;
        }

        if (!stage_loaded[i] && (int)i >= bottom_stage_id && (int)i <= top_stage_id) {
            stages[i] = new Stage(this, i, this->stage_scaling);
            stage_loaded[i] = true;
        }

        if (stage_loaded[i]) {
            stages[i]->setPosition(0, stage_scroll_offset);
            stage_scroll_offset -= stage_size.y;
        }
    }
}

bool StageScreen::onMouseScroll(sf::Event &event, void *data) {
    StageScreen *obj = (StageScreen *)data;

    obj->y_scroll += event.mouseWheelScroll.delta * 30;

    obj->constrainScroll();

    obj->calculateStagePositions();
    return true;
}

void StageScreen::constrainScroll() {
    int max_scroll = (this->num_stages - 1) * this->stage_size.y - (this->view_size.y - this->stage_size.y);

    if (this->y_scroll < 0)
        this->y_scroll = 0;
    if (this->y_scroll > max_scroll)
        this->y_scroll = max_scroll;
}

void StageScreen::setup() {
    //this->view_size = (sf::Vector2f)this->getView().getSize();

    btn_exit.setPosition(view_size.x - 70, 10);

    sf::Vector2f window_size = (sf::Vector2f)this->getSize();
    setView(sf::View(sf::FloatRect(0, 0, window_size.x, window_size.y)));

    this->stage_scaling = this->view_size.x / this->original_stage_size.x;
    this->stage_size = {(int)(this->original_stage_size.x * this->stage_scaling), (int)(this->original_stage_size.y * this->stage_scaling)};

    for (size_t i = 0; i < stage_loaded.size(); i++) {
        if (stage_loaded[i]) {
            stage_loaded[i] = false;
            delete stages[i];
        }
    }

    calculateStagePositions();
}

void StageScreen::render() {
    // Clear screen
    clear(GameStyle::GRAY);

    for (size_t i = 0; i < stage_loaded.size(); i++) {
        if (stage_loaded[i])
            stages[i]->render();
    }

    btn_exit.render();
}