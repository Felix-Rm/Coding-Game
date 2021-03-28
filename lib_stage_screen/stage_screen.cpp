#include "stage_screen.h"

StageScreen::StageScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : SelectionScreen(video_mode, title, style) {
    this->stage_path = "_assets/stages/";
    this->onOpenLevel = StageScreen::run_level;

    std::ifstream stage_info("_assets/stages.info");
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

bool StageScreen::onMouseScroll(sf::Event &event, void *data) {
    StageScreen *obj = (StageScreen *)data;

    obj->onMouseScroll_selection(event, data);
    return true;
}

bool StageScreen::run_level(sf::Event &event, void *data) {
    auto *info = (std::pair<Stage *, int> *)data;

    LevelScreen window{info->first->getWindow().getVideoMode(), "Level " + std::to_string(info->second), info->first->getWindow().getStyle(), info->second, info->first->path};

    info->first->getWindow().setVisible(false);
    while (window.run())
        ;

    info->first->getWindow().setVisible(true);
    return true;
}

void StageScreen::setup() {
    btn_exit.setPosition(view_size.x - 70, 10);

    setup_selection();
}

void StageScreen::render() {
    render_selection();

    btn_exit.render();
}