#include <stdio.h>
#include <unistd.h>

#include <iostream>

#include "lib_level_editor_screen/level_editor_screen.h"
#include "lib_level_screen/level_screen.h"
#include "lib_sandbox_screen/sandbox_screen.h"
#include "lib_title_screen/title_screen.h"
#include "lib_window/style.h"
#include "lib_window/window.h"

int main() {
    if (!GameStyle::setup())
        throw std::runtime_error("[GameStyle] could not load resources!");

    TitleScreen window{};
    //StageScreen window{sf::VideoMode::getDesktopMode(), "Campaign", sf::Style::Default};
    //SandboxScreen window{sf::VideoMode::getDesktopMode(), "Sandbox", sf::Style::Default};

    // std::string level_path = "_assets/stages/0/";
    // LevelScreen window{
    //     sf::VideoMode::getDesktopMode(),
    //     "Level #",
    //     sf::Style::Default,
    //     0, level_path};

    // std::string level_path = "_assets/stages/0/";
    // LevelEditorScreen window{
    //     sf::VideoMode::getDesktopMode(),
    //     "Level #",
    //     sf::Style::Default,
    //     0, level_path};

    while (window.run())
        ;

    return 0;
}