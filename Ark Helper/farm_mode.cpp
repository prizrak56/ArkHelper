#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <thread>

#include "string_processing.h"
#include "utils.h"
#include "farm_mode.h"

FarmMode::FarmMode(HWND h_window_handle) : h_window_handle_(h_window_handle){}

FarmMode::FarmMode(std::string file_with_setting, HWND& h_window_handle) : search_drop_button_file_path_(file_with_setting),h_window_handle_(h_window_handle){}

void FarmMode::ToggleResource(bool& resource){

    if (resource == true) {
        resource = false;
    }
    else {
        resource = true;
    }
    resources = Resources::NONE;
    Sleep(100);
}

void FarmMode::ApplyOrInitializeSettingsFromFile(){

    if (h_window_handle_ == NULL) {
        system("cls");
        MenuMessage();
        std::cout << "Process not found" << std::endl;
        mode_selection.second = Command::NONE;
        EnableDisableFunc(Command::NONE);
        return;
    }
    Point tmp_search;
    Point tmp_drop;

    std::ifstream file_name;

    if (FileExists(search_drop_button_file_path_)) {

        file_name.open(search_drop_button_file_path_);

        std::string line;
        int line_number = 0;
        while (std::getline(file_name, line)) {

            if (line_number == 0) {
                tmp_search.x = std::stoi(line);
            }
            else if (line_number == 1) {
                tmp_search.y = std::stoi(line);
            }
            else if (line_number == 2) {
                tmp_drop.x = std::stoi(line);
            }
            else if (line_number == 3) {
                tmp_drop.y = std::stoi(line);
            }
            ++line_number;
        }
        file_name.close();
    }
    else {
        std::cout << "press F1 first on the search window for the right inventory,\n then press F1 on the button for throwing out all items in the right inventory\n";

        GetSetCursorPosition(tmp_search.x, tmp_search.y, tmp_drop.x, tmp_drop.y);

        std::ofstream out;
        out.open(search_drop_button_file_path_);
        out << tmp_search.x << "\n" << tmp_search.y << "\n" << tmp_drop.x << "\n" << tmp_drop.y;
        out.close();
    }
    drop_button_.x = tmp_drop.x;
    drop_button_.y = tmp_drop.y;
    search_button_.x = tmp_search.x;
    search_button_.y = tmp_search.y;
}

void FarmMode::GetSetCursorPosition(int& x1, int& y1, int& x2, int& y2) {
    POINT* p_cursor_pos = new POINT;
    int i = 0;

    std::cout << "F1 - Get cursor position\n";
    while (i < 2) {
        if (GetAsyncKeyState(VK_F1)) {
            ++i;

            Beep(200, 200);
            if (GetCursorPos(p_cursor_pos)) {
                if (i == 1) {
                    x1 = p_cursor_pos->x;
                    y1 = p_cursor_pos->y;
                }
                if (i == 2) {
                    x2 = p_cursor_pos->x;
                    y2 = p_cursor_pos->y;
                }
                std::cout << "success\n";
            }
            else {
                std::cout << "Failed to get cursor position" << std::endl;
            }
            Sleep(50);
        }
    }
}

void FarmMode::ClickToSearchInventory() {
    PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(search_button_.x, search_button_.y));
    PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(search_button_.x, search_button_.y));
}

void FarmMode::ClickToDrop() {
    PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(drop_button_.x, drop_button_.y));
    PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(drop_button_.x, drop_button_.y));
}

void FarmMode::Drop(std::string name) {

    ClickToSearchInventory();
    Sleep(100);
    for (auto& a : name) {
        PostMessage(h_window_handle_, WM_KEYDOWN, a, 0);
        Sleep(20);
    }
    Sleep(400);
    ClickToDrop();
    Sleep(200);
}

void FarmMode::HandleInventory() {
    Sleep(400);
    std::cout << "Press F\n";
    PostMessage(h_window_handle_, WM_KEYDOWN, 'F', 0);
    Sleep(1000);

    if (b_stone_ == true) {
        std::cout << "DropStone()\n";
        Drop(stone_);
        Sleep(100);
    }
    if (b_flint_ == true) {
        std::cout << "DropFlint()\n";
        Drop(flint_);
        Sleep(100);
    }
    if (b_berry_ == true) {
        std::cout << "DropBerry()\n";
        Drop(berry_);
        Sleep(100);
    }
    if (b_thatch_ == true) {
        std::cout << "DropThatch()\n";
        Drop(thatch_);
        Sleep(100);
    }
    if (b_wood_ == true) {
        std::cout << "DropWood()\n";
        Drop(wood_);
        Sleep(100);
    }
    if (b_sand_ == true) {
        std::cout << "DropSand()\n";
        Drop(sand_);
    }
    if (b_metal_ == true) {
        std::cout << "DropMetal()\n";
        Drop(metal_);
    }
    PostMessage(h_window_handle_, WM_KEYDOWN, VK_ESCAPE, 0);
}

void FarmMode::EditCoordsPos() {

    ClearFile(search_drop_button_file_path_);
    system("cls");

    std::cout << "press F1 first on the search window for the right inventory,\n then press F1 on the button for throwing out all items in the right inventory\n";

    GetSetCursorPosition(search_button_.x, search_button_.y, drop_button_.x, drop_button_.y);

    std::ofstream out;
    out.open(search_drop_button_file_path_);
    out << search_button_.x << "\n" << search_button_.y << "\n" << drop_button_.x << "\n" << drop_button_.y;
    out.close();
}

void FarmMode::FarmStart() {

    ApplyOrInitializeSettingsFromFile();
    mode_selection.second = Command::SELECT_SETTINGS;

    int seconds = 100;
    int seconds_2 = 0;

    while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
        system("cls");

        std::cout << "F1 - Start\n\n";
        std::cout << "Seconds: " << seconds << "\n";
        std::cout << "Throw away.\n";
        std::cout << std::boolalpha << "F2 - Flint " << b_flint_ << "\n";
        std::cout << std::boolalpha << "F3 - Stone " << b_stone_ << "\n";
        std::cout << std::boolalpha << "F4 - Wood " << b_wood_ << "\n";
        std::cout << std::boolalpha << "F5 - Berry " << b_berry_ << "\n";
        std::cout << std::boolalpha << "F6 - Thatch " << b_thatch_ << "\n";
        std::cout << std::boolalpha << "F7 - Sand " << b_sand_ << "\n";
        std::cout << std::boolalpha << "F8 - Metal " << b_metal_ << "\n";
        std::cout << "F9 - Set clicking time before reset in seconds\n";

        switch (resources)
        {
        case Resources::NONE:
            break;
        case Resources::DELAY:
            system("cls");
            std::cout << "Enter number of seconds: ";
            std::cin >> seconds;
            std::cout << "OK == OK\n";
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

    while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {

        while (seconds > seconds_2) {
            PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
            if (mode_selection.first != Command::FARM_MODE || mode_selection.second != Command::START) {
                break;
            }
            system("cls");
            std::cout << "F2 - disable\n";
            std::cout << seconds - seconds_2 << "\n";
            ++seconds_2;
            Sleep(100);
        }

        seconds_2 = 0;
        if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
            std::cout << "Droping\n";
            HandleInventory();
        }
    }
    system("cls");
    MenuMessage();
}
