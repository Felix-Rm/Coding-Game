#include "campaign_screen.h"

CampaignScreen::CampaignScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : SelectionScreen(video_mode, title, style) {
    this->stage_load_path = "_assets/stages/";
    this->stage_save_path = "_user/gamesave/_assets/stages/";

    this->onCreateLevel = run_level;

    std::ifstream stage_info("_assets/stages.info");
    if (!stage_info)
        throw std::runtime_error("could not open stage info");

    stage_info >> this->num_stages >> this->original_stage_size.x >> this->original_stage_size.y;
    stage_info.close();

    for (int i = 0; i < num_stages; i++)
        stage_loaded.push_back(false);

    this->btn_exit = ImageButton(this, {0, 0}, {60, 60}, GameStyle::Icon::EXIT);
    this->btn_exit.setImageScale(0.7);
    this->btn_exit.setBgColor(GameStyle::RED);
    this->btn_exit.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this,
                                                                                       [](sf::Event &event, void *data) {
                                                                                           ((Window *)data)->close();
                                                                                           return true;
                                                                                       }));

    setup();

    addEventHandler(onMouseScroll, this, 1, sf::Event::EventType::MouseWheelScrolled);
}

bool CampaignScreen::onMouseScroll(sf::Event &event, void *data) {
    CampaignScreen *obj = (CampaignScreen *)data;

    obj->onMouseScroll_selection(event, data);
    return true;
}

bool CampaignScreen::run_level(sf::Event &event, void *data) {
    Stage::level_creation_data *creation_data = (Stage::level_creation_data *)data;

    *(creation_data->level_window) = new LevelScreen(creation_data->selection_window->getVideoMode(), "Level " + std::to_string(creation_data->level_id), creation_data->selection_window->getStyle(), creation_data->level_id, creation_data->path);

    return true;
}

void CampaignScreen::setup() {
    btn_exit.setPosition(view_size.x - 70, 10);

    setup_selection();
}

void CampaignScreen::render() {
    render_selection();

    btn_exit.render();
}