#include <cstddef>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "farm_mode.h"
#include "string_processing.h"
#include "utils.h"

namespace farm {
// ---------------------------------- [Farm Mode] Realization -------------------------
//                                                                                    +
//                                                                                    + ---------------------------
// ------------------------------------------------------------------------------------ Constuctors & Auxiliaries +

    FarmMode::FarmMode(HWND h_window_handle) : h_window_handle_(h_window_handle) {}
    FarmMode::FarmMode(const std::filesystem::path& file_with_setting, HWND& h_window_handle) : path_settings_(file_with_setting), h_window_handle_(h_window_handle) {}

    std::filesystem::path operator""_p(const char* ch, std::size_t size) {
        return std::filesystem::path(ch, ch + size);
    }

// 
// 
//                                                                                    + ---------------------------
// ------------------------------------------------------------------------------------ Farm Mode Private Methods +

    void FarmMode::ApplyOrInitializeSettingsFromFile() {

        if (h_window_handle_ == NULL) {
            system("cls");
            MenuMessage();
            std::cout << "The process was not found."s << std::endl;
            mode_selection.second = Command::NONE;
            EnableDisableFunc(Command::NONE);
            return;
        }

        std::ifstream input(path_settings_, std::ios::in);

        if (!input.is_open()) {
            std::cout << "The file was not created or opened: "s << path_settings_ << std::endl;
            return;
        }

        std::string line;
        std::size_t counter = 0;

        while (std::getline(input, line)) {
            if (counter == 27) {
                auto [x, y] = ParseCoords(std::move(line));
                drop_button_.x = x;
                drop_button_.y = y;
            }

            if (counter == 28) {
                auto [x, y] = ParseCoords(std::move(line));
                search_button_.x = x;
                search_button_.y = y;
            }
            ++counter;
        }
        input.close();
    }

    void FarmMode::ClickToDrop() {
        PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(drop_button_.x, drop_button_.y));
        PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(drop_button_.x, drop_button_.y));
    }

    void FarmMode::ClickToSearchInventory() {
        PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(search_button_.x, search_button_.y));
        PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(search_button_.x, search_button_.y));
    }

    void FarmMode::Drop(std::string_view resource) {

        ClickToSearchInventory();
        Sleep(100);

        for (const auto& letter : resource) {
            PostMessage(h_window_handle_, WM_KEYDOWN, letter, 0);
            Sleep(20);
        }

        Sleep(400);
        ClickToDrop();
        Sleep(200);
    }

    void FarmMode::HandleInventory() {
        Sleep(400);
        std::cout << "Press F"s << std::endl;

        PostMessage(h_window_handle_, WM_KEYDOWN, 'F', 0);
        Sleep(1000);

        if (b_stone_ == true) {
            std::cout << "DropStone()"s << std::endl;
            Drop(stone_);
            Sleep(100);
        }
        if (b_flint_ == true) {
            std::cout << "DropFlint()"s << std::endl;
            Drop(flint_);
            Sleep(100);
        }
        if (b_berry_ == true) {
            std::cout << "DropBerry()"s << std::endl;
            Drop(berry_);
            Sleep(100);
        }
        if (b_thatch_ == true) {
            std::cout << "DropThatch()"s << std::endl;
            Drop(thatch_);
            Sleep(100);
        }
        if (b_wood_ == true) {
            std::cout << "DropWood()"s << std::endl;
            Drop(wood_);
            Sleep(100);
        }
        if (b_sand_ == true) {
            std::cout << "DropSand()"s << std::endl;
            Drop(sand_);
        }
        if (b_metal_ == true) {
            std::cout << "DropMetal()"s << std::endl;
            Drop(metal_);
        }

        PostMessage(h_window_handle_, WM_KEYDOWN, VK_ESCAPE, 0);
    }

    void FarmMode::SelectResources(int first_timer, int second_timer) {
        
        while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
            system("cls");

            std::cout << "F1 - Start"s << std::endl << std::endl;
            std::cout << "Seconds: "s << first_timer << std::endl;
            std::cout << "Throw away."s << std::endl;
            std::cout << std::boolalpha << "F2 - Flint "s << b_flint_ << std::endl;
            std::cout << std::boolalpha << "F3 - Stone "s << b_stone_ << std::endl;
            std::cout << std::boolalpha << "F4 - Wood "s << b_wood_ << std::endl;
            std::cout << std::boolalpha << "F5 - Berry "s << b_berry_ << std::endl;
            std::cout << std::boolalpha << "F6 - Thatch "s << b_thatch_ << std::endl;
            std::cout << std::boolalpha << "F7 - Sand "s << b_sand_ << std::endl;
            std::cout << std::boolalpha << "F8 - Metal "s << b_metal_ << std::endl;
            std::cout << "F9 - Set clicking time before reset in seconds"s << std::endl;

            switch (resources) {
            case Resources::NONE:
                break;

            case Resources::DELAY:
                system("cls");
                std::cout << "Enter number of seconds: "s;
                std::cin >> first_timer;
                std::cout << "OK == OK"s << std::endl;
                system("cls");
                resources = Resources::NONE;
                Sleep(500);

                break;

            case Resources::FLINT:
                ToggleResource(b_flint_);
                break;

            case Resources::STONE:
                ToggleResource(b_stone_);
                break;

            case Resources::WOOD:
                ToggleResource(b_wood_);
                break;

            case Resources::BERRY:
                ToggleResource(b_berry_);
                break;

            case Resources::THATCH:
                ToggleResource(b_thatch_);
                break;

            case Resources::SAND:
                ToggleResource(b_sand_);
                break;

            case Resources::METAL:
                ToggleResource(b_metal_);
                break;
            }

            Sleep(100);
        }
    }

    void FarmMode::ToggleResource(bool& resource) {
        if (resource == true) {
            resource = false;
        }
        else {
            resource = true;
        }

        resources = Resources::NONE;
        Sleep(100);
    }

// 
// 
//                                                                                    + ---------------------
// ------------------------------------------------------------------------------------ Farm Mode Interface +

    void FarmMode::StartFarm() {

        ApplyOrInitializeSettingsFromFile();
        mode_selection.second = Command::SELECT_SETTINGS;

        int first_timer = 100;
        int second_timer = 0;
        SelectResources(first_timer, second_timer);

        while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {

            while (first_timer > second_timer) {
                PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));

                if (mode_selection.first != Command::FARM_MODE || mode_selection.second != Command::START) {
                    break;
                }

                system("cls");
                std::cout << "F2 - disable"s << std::endl;
                std::cout << first_timer - second_timer << std::endl;
                ++second_timer;

                Sleep(100);
            }

            second_timer = 0;
            if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
                std::cout << "Dropping"s << std::endl;
                HandleInventory();
            }
        }
        system("cls");

        MenuMessage();
    }
    
    void FarmMode::EditCoords() {
        std::ifstream input(path_settings_, std::ios::in);

        if (!input.is_open()) {
            std::cout << "The file was not created or opened: "s << path_settings_ << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string to_read;

        while (std::getline(input, to_read)) {
            lines.push_back(std::move(to_read));
        }
        input.close();

        std::cout << "Specify coordinates of the \"Drop everything\" button."s << std::endl;
        GetSetCursorPosition(drop_button_.x, drop_button_.y);

        std::cout << "Specify coordinates of the \"Search window\" button."s << std::endl;
        GetSetCursorPosition(search_button_.x, search_button_.y);

        lines[27] = "drop_everything: "s + std::to_string(drop_button_.x) + ", " + std::to_string(drop_button_.y);
        lines[28] = "search_window: "s + std::to_string(search_button_.x) + ", " + std::to_string(search_button_.y);

        std::ofstream out(path_settings_, std::ios::out);

        if (!out.is_open()) {
            std::cout << "The file was not created or opened: "s << path_settings_ << std::endl;
            return;
        }

        for (const auto& line : lines) {
            out << line << std::endl;
        }
        out.close();
    }
} // namespace farm