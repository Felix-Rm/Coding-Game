#include "sandbox_screen.h"

SandboxScreen::SandboxScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : SelectionScreen(video_mode, title, style) {
    this->stage_load_path = "_user/stages/";
    this->stage_save_path = "_user/gamesave/_user/stages/";

    this->onCreateLevel = run_level;

    std::ifstream stages_info("_user/stages.info");
    if (!stages_info)
        throw std::runtime_error("[Sandbox] Could not open stages info: _user/stages.info");

    stages_info >> this->num_stages >> this->original_stage_size.x >> this->original_stage_size.y;
    stages_info.close();

    for (int i = 0; i < num_stages; i++)
        stage_loaded.push_back(false);

    this->btn_exit = ImageButton(this, {0, 0}, {60, 60}, GameStyle::Icon::EXIT);
    this->btn_exit.setImageScale(0.7);
    this->btn_exit.setOutline(1);
    this->btn_exit.setBgColor(GameStyle::RED);
    this->btn_exit.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this,
                                                                                       [](sf::Event &event, void *data) {
                                                                                           ((Window *)data)->close();
                                                                                           return true;
                                                                                       }));

    this->btn_edit = ImageButton(this, {0, 0}, {60, 60}, GameStyle::Icon::EDIT);
    this->btn_edit.setImageScale(0.7);
    this->btn_edit.setOutline(1);
    this->btn_edit.setBgColor(GameStyle::GRAY);
    this->btn_edit.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, onEditToggle));

    setup();

    addEventHandler(onMouseScroll, this, 1, sf::Event::EventType::MouseWheelScrolled);
}

bool SandboxScreen::onMouseScroll(sf::Event &event, void *data) {
    SandboxScreen *obj = (SandboxScreen *)data;

    obj->onMouseScroll_selection(event, data);
    return true;
}

bool SandboxScreen::onEditToggle(sf::Event &event, void *data) {
    SandboxScreen *obj = (SandboxScreen *)data;
    obj->edit_activated = !obj->edit_activated;

    for (auto &stage : obj->stages) stage.second->activateEditMode(obj->edit_activated);

    obj->btn_edit.setBgColor(obj->edit_activated ? GameStyle::GREEN : GameStyle::GRAY);
    return true;
}

void SandboxScreen::onStageLoad(Stage *stage) {
    stage->activateEditMode(edit_activated);
}

bool SandboxScreen::run_level(sf::Event &event, void *data) {
    Stage::level_creation_data *creation_data = (Stage::level_creation_data *)data;
    SandboxScreen *obj = (SandboxScreen *)creation_data->selection_window;

    if (obj->edit_activated)
        *(creation_data->level_window) = new LevelEditorScreen(creation_data->selection_window->getVideoMode(), "Level " + std::to_string(creation_data->level_id), creation_data->selection_window->getStyle(), creation_data->level_id, creation_data->path);
    else {
        *(creation_data->level_window) = new LevelScreen(creation_data->selection_window->getVideoMode(), "Level " + std::to_string(creation_data->level_id), creation_data->selection_window->getStyle(), creation_data->level_id, creation_data->path);

        // don't look at this
        (*((LevelScreen **)creation_data->level_window))->activateManualControlls();
    }
    return true;
}

void SandboxScreen::setup() {
    btn_exit.setPosition(view_size.x - 70, 10);
    btn_edit.setPosition(view_size.x - 140, 10);

    setup_selection();
}

void SandboxScreen::render() {
    render_selection();

    btn_exit.render();
    btn_edit.render();
}