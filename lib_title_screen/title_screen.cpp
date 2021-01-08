#include "title_screen.h"

TitleScreen::TitleScreen() : Window(sf::VideoMode::getDesktopMode(), "Info Game", sf::Style::Default) {
    setup();
}

void TitleScreen::setup() {
    sf::Vector2f size = (sf::Vector2f)this->getView().getSize();

    setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));

    framerate_display = sf::Text("0", game_font, 20);
    framerate_display.setPosition({0, 0});

    std::string button_names[] = {"Kampagne", "Sandbox", "Einstellungen", "Beenden"};

    Window::event_handler_t button_event_handlers[] = {
        Window::createEventHandler(runCampaign, this),
        Window::createEventHandler(Window::noop, nullptr),
        Window::createEventHandler(Window::noop, nullptr),
        Window::createEventHandler([](sf::Event& event, void* data) {
            ((Window*)data)->close();
            return true;
        },
                                   this)};

    int button_names_length = sizeof(button_names) / sizeof(button_names[0]);
    sf::Vector2f button_size = {850, 120};
    int button_text_size = 90;
    int button_outline_thickness = 4;
    int button_vertical_spacing = 50;

    int vertical_start = size.y / 2 - (button_size.y * button_names_length + button_vertical_spacing * (button_names_length - 1)) / 2;

    if (buttons.size() == 0)
        for (int i = 0; i < button_names_length; i++)
            buttons.push_back(nullptr);

    for (int i = 0; i < button_names_length; i++) {
        if (buttons[i] != nullptr) delete buttons[i];
        buttons[i] = new Button(this, {size.x / 2 - button_size.x / 2, vertical_start + i * (button_size.y + button_vertical_spacing)}, button_size, button_names[i], button_text_size, button_outline_thickness, game_colors::LIGHT_GRAY, game_colors::ORANGE, button_event_handlers[i]);
    }
}

bool TitleScreen::run() {
    // Process events
    checkEvents();

    // Clear screen
    clear(game_colors::GRAY);

    framerate = (framerate * 10 + 1.f / clock.getElapsedTime().asSeconds()) / 11;
    clock.restart();

    char framerate_text[15];
    sprintf(framerate_text, "%06.2f", framerate);
    framerate_display.setString(framerate_text);

    draw(framerate_display);

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->render(this);
    }

    display();

    return isOpen();
}

bool TitleScreen::runCampaign(sf::Event& event, void* data) {
    TitleScreen* obj = (TitleScreen*)data;

    LevelScreen level_screen{sf::VideoMode::getDesktopMode(), "Campaign", sf::Style::Default};

    obj->setVisible(false);

    while (level_screen.run())
        ;

    obj->setVisible(true);

    return true;
}