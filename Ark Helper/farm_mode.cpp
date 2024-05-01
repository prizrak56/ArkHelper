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
        InfoMessage();
        std::cout << "Process not found" << std::endl;
        mode_selection.second = Command::NONE;
        EnableDisableFunc(Command::NONE);
        return;
    }

    int s_x = 0, s_y = 0, d_x = 0, d_y = 0;
    std::ifstream file_name;

    if (FileExists()) {

        file_name.open(search_drop_button_file_path_);

        std::string line;
        int line_number = 0;
        while (std::getline(file_name, line)) {

            if (line_number == 0) {
                s_x = std::stoi(line);
            }
            else if (line_number == 1) {
                s_y = std::stoi(line);
            }
            else if (line_number == 2) {
                d_x = std::stoi(line);
            }
            else if (line_number == 3) {
                d_y = std::stoi(line);
            }
            ++line_number;
        }
        file_name.close();
    }
    else {
        std::cout << "press F1 first on the search window for the right inventory,\n then press F1 on the button for throwing out all items in the right inventory\n";

        GetSetCursorPosition(s_x, s_y, d_x, d_y);

        std::ofstream out;
        out.open(search_drop_button_file_path_);
        out << s_x << "\n" << s_y << "\n" << d_x << "\n" << d_y;
        out.close();
    }
    drop_x_ = d_x;
    drop_y_ = d_y;
    search_x_ = s_x;
    search_y_ = s_y;
}

bool FarmMode::FileExists() {
    std::ifstream file(search_drop_button_file_path_);
    return file.good();
}

void FarmMode::ClearFile() const{
    std::ofstream ofs;
    ofs.open(search_drop_button_file_path_, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void FarmMode::GetSetCursorPosition(int& x1, int& y1, int& x2, int& y2) {
    POINT* pCursorPos = new POINT;
    int i = 0;

    std::cout << "F1 - Get cursor position\n";
    while (i < 2) {
        if (GetAsyncKeyState(VK_F1)) {
            ++i;

            Beep(200, 200);
            if (GetCursorPos(pCursorPos)) {
                if (i == 1) {
                    x1 = pCursorPos->x;
                    y1 = pCursorPos->y;
                }
                if (i == 2) {
                    x2 = pCursorPos->x;
                    y2 = pCursorPos->y;
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
    PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(search_x_, search_y_));
    PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(search_x_, search_y_));
}

void FarmMode::ClickToDrop() {
    PostMessage(h_window_handle_, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(drop_x_, drop_y_));
    PostMessage(h_window_handle_, WM_LBUTTONUP, 0, MAKELPARAM(drop_x_, drop_y_));
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

    ClearFile();
    system("cls");

    std::cout << "press F1 first on the search window for the right inventory,\n then press F1 on the button for throwing out all items in the right inventory\n";

    GetSetCursorPosition(search_x_, search_y_, drop_x_, drop_y_);

    std::ofstream out;
    out.open(search_drop_button_file_path_);
    out << search_x_ << "\n" << search_y_ << "\n" << drop_x_ << "\n" << drop_y_;
    out.close();
}

void FarmMode::FarmStart() {

    ApplyOrInitializeSettingsFromFile();

    system("cls");

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
    InfoMessage();
}
