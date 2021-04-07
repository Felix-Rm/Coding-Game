#include "title_screen.h"

TitleScreen::TitleScreen() : Window(sf::VideoMode::getDesktopMode(), "Info Game", sf::Style::Default) {
    setup();
}

void TitleScreen::setup() {
    std::string button_names[] = {"Campaign", "Sandbox", "Settings", "Exit"};

    Window::event_handler_t button_event_handlers[] = {
        Window::createEventHandler(runCampaign, this),
        Window::createEventHandler(runSandbox, this),
        Window::createEventHandler(Window::event_noop, nullptr),
        Window::createEventHandler([](sf::Event &event, void *data) {
            ((Window *)data)->close();
            return true;
        },
                                   this)};

    int button_names_length = sizeof(button_names) / sizeof(button_names[0]);
    sf::Vector2f button_size = {850, 120};
    int button_text_size = 90;
    int button_outline_thickness = 4;
    int button_vertical_spacing = 50;

    int vertical_start = this->view_size.y / 2 - (button_size.y * button_names_length + button_vertical_spacing * (button_names_length - 1)) / 2;

    buttons.clear();

    for (int i = 0; i < button_names_length; i++) {
        sf::Vector2f pos = {this->view_size.x / 2 - button_size.x / 2, vertical_start + i * (button_size.y + button_vertical_spacing)};
        buttons.push_back(TextButton(this, pos, button_size, button_names[i], button_text_size, button_outline_thickness, GameStyle::LIGHT_GRAY, GameStyle::ORANGE, button_event_handlers[i]));
    }
}

void TitleScreen::render() {
    // Clear screen
    clear(GameStyle::DARK_GRAY);

    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].render();
    }
}

bool TitleScreen::runCampaign(sf::Event &event, void *data) {
    TitleScreen *obj = (TitleScreen *)data;
    StageScreen stage_screen{obj->video_mode, "Campaign", obj->style};

    obj->setVisible(false);

    while (stage_screen.run())
        ;

    obj->setVisible(true);

    return true;
}

bool TitleScreen::runSandbox(sf::Event &event, void *data) {
    TitleScreen *obj = (TitleScreen *)data;
    SandboxScreen sandbox_screen{obj->video_mode, "Sandbox", obj->style};

    obj->setVisible(false);

    while (sandbox_screen.run())
        ;

    obj->setVisible(true);

    return true;
}