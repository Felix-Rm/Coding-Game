#include "sandbox_screen.h"

SandboxScreen::SandboxScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : SelectionScreen(video_mode, title, style) {
    this->stage_path = "_user/stages/";
    this->onOpenLevel = SandboxScreen::run_level;

    std::ifstream stages_info("_user/stages.info");
    if (!stages_info)
        throw std::runtime_error("[Sandbox] Could not open stages info: _user/stages.info");

    stages_info >> this->num_stages >> this->original_stage_size.x >> this->original_stage_size.y;
    stages_info.close();

    for (int i = 0; i < num_stages; i++)
        stage_loaded.push_back(false);

    this->btn_exit = ImageButton(this, {0, 0}, {60, 60},
                                 GameStyle::Icon::EXIT, 0.7, 1, GameStyle::WHITE, GameStyle::RED, Window::createEventHandler([](sf::Event &event, void *data) {
                                     ((Window *)data)->close();
                                     return true;
                                 },
                                                                                                                             this));

    this->btn_edit = ImageButton(this, {0, 0}, {60, 60}, GameStyle::Icon::EDIT, 0.7, 1, GameStyle::WHITE, GameStyle::GRAY, Window::createEventHandler(onEditToggle, this));

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

    obj->btn_edit.setBgColor(obj->edit_activated ? GameStyle::GREEN : GameStyle::GRAY);
    return true;
}

bool SandboxScreen::run_level(sf::Event &event, void *data) {
    auto *info = (std::pair<Stage *, int> *)data;

    SandboxScreen *obj = (SandboxScreen *)&info->first->getWindow();
    LevelScreen *window;

    if (obj->edit_activated)
        window = new LevelEditorScreen{info->first->getWindow().getVideoMode(), "Level " + std::to_string(info->second), info->first->getWindow().getStyle(), info->second, info->first->path};
    else {
        window = new LevelScreen{info->first->getWindow().getVideoMode(), "Level " + std::to_string(info->second), info->first->getWindow().getStyle(), info->second, info->first->path};
        window->activateManualControlls();
    }
    obj->setVisible(false);
    while (window->run())
        ;

    obj->setVisible(true);
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