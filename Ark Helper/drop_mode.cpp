#include "drop_mode.h"
#include "string_processing.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std::literals;

DropMode::DropMode(std::string& settings_path) : path_settings_(settings_path) {
    SetSettings();
}

void DropMode::MouseDown() {
    std::cout << "Mouse down start"s << std::endl;
    for (int i = 0; i < 100; i++) {
        mouse_event(MOUSEEVENTF_MOVE, 0, 10, 0, 0);  //full down
    }
    std::cout << "Mouse down end"s << std::endl;
    Sleep(200);
}

void DropMode::OpenTp() {
    std::cout << "Open teleport start"s << std::endl;
    SimulateKeyPress('E');
    std::cout << "Open teleport end"s << std::endl;
    Sleep(1000);
}

void DropMode::SearchClick(Point search_window) {
    std::cout << "Search click start" << std::endl;
    SetCursorPos(search_window.x, search_window.y);
    Sleep(500);
    left_button_.Click();
    std::cout << "Search click end"s << std::endl;
    Sleep(200);
}

void DropMode::EnterTpName(std::string& teleport_name) {

    std::cout << "Enter tp name start"s << std::endl;
    for (auto a : teleport_name) {
        SimulateKeyPress(a);   //enter tp_name
        Sleep(50);
    }
    std::cout << "Enter tp name end"s << std::endl;
    Sleep(200);
}

void DropMode::ClickToTpName(Point tp_name_s) {
    std::cout << "Click to tp name start"s << std::endl;
    Sleep(500);
    SetCursorPos(tp_name_s.x, tp_name_s.y);
    Sleep(500);
    left_button_.Click();
    std::cout << "Click to tp name end"s << std::endl;
    Sleep(200);
}

void DropMode::ClickToTeleporting(Point teleporting) {
    std::cout << "teleporting start" << std::endl;
    SetCursorPos(teleporting.x, teleporting.y); //teleport
    Sleep(500);
    left_button_.Click();
    std::cout << "teleporting end"s << std::endl;
    Sleep(200);
}

void DropMode::MouseUp() {

    std::cout << "Mouse up start"s << std::endl;
    for (int i = 0; i < 4/*67*/; i++) {
        mouse_event(MOUSEEVENTF_MOVE, 0, -10, 0, 0);
    }
    std::cout << "Mouse up end"s << std::endl;
    Sleep(200);
}

void DropMode::MoveTo360() {

    std::cout << "360 with E start"s << std::endl;
    for (int i = 0; i < 380; i++) {
        mouse_event(MOUSEEVENTF_MOVE, -10, 0, 0, 0);
        SimulateKeyPress('E');
        Sleep(20);
    }
    std::cout << "360 with E end"s << std::endl;
    Sleep(200);
}

void DropMode::TakeAll(Point take_all) {
    std::cout << "Take all start"s << std::endl;
    SetCursorPos(take_all.x, take_all.y);   // take everything
    //mouse_click();
    left_button_.Click();
    std::cout << "Take all end"s << std::endl;
    Sleep(500);
}

void DropMode::GiveAll(Point give_all) {
    std::cout << "Drop in vault start"s << std::endl;
    SetCursorPos(give_all.x, give_all.y);    // put everything
    left_button_.Click();
    std::cout << "Drop in vault end\n";
    Sleep(500);
}

void DropMode::CloseInventory(Point close_invetory) {
    std::cout << "Close inventory start"s << std::endl;
    SetCursorPos(close_invetory.x, close_invetory.y);
    left_button_.Click();
    std::cout << "Close inventory end"s << std::endl;
    Sleep(500);
}

void DropMode::LootingCrete(std::string& current_tp_name, bool take) {

    system("cls");
    Sleep(5000);
    MouseDown();

    OpenTp();

    SearchClick(settings_.search_window);

    EnterTpName(current_tp_name);

    ClickToTpName(settings_.first_tp_name_in_list);

    ClickToTeleporting(settings_.teleporting);

    std::cout << "Sleep 10s"s << std::endl;
    Sleep(10000);

    MouseUp();

    MoveTo360();
    if (take == true) {
        TakeAll(settings_.take_all);
    }
    if (take == false) {
        GiveAll(settings_.give_all);
    }
    CloseInventory(settings_.close_invetory);

    std::cout << "Sleep 10s"s << std::endl;
    Sleep(10000);
}

void DropMode::SetSettings() {

    std::ifstream input;
    input.open(path_settings_);
    if (!input.is_open()) {
        std::cout << "settings were not saved file could not open file"s << std::endl;
        return;
    }
    std::string line;
    std::size_t counter = 0;
    bool settings_is_found = false;

    while (std::getline(input, line)) {

        if (settings_is_found == true && line != "") {

            if (counter == 7 || counter == 8) {
                std::vector<std::string> tp_names = ParseName(line);
                if (counter == 7) {
                    teleports_name_ = std::move(tp_names);
                }
                else {
                    save_teleport_name_ = tp_names.at(0);
                }
                ++counter;
                continue;
            }

            if (counter != 6) {
                std::pair<int, int> coords = ParseCoords(line);

                switch (counter) {
                case 0:
                    settings_.search_window.x = coords.first;
                    settings_.search_window.y = coords.second;
                    break;
                case 1:
                    settings_.first_tp_name_in_list.x = coords.first;
                    settings_.first_tp_name_in_list.y = coords.second;
                    break;
                case 2:
                    settings_.teleporting.x = coords.first;
                    settings_.teleporting.y = coords.second;
                    break;
                case 3:
                    settings_.take_all.x = coords.first;
                    settings_.take_all.y = coords.second;
                    break;
                case 4:
                    settings_.give_all.x = coords.first;
                    settings_.give_all.y = coords.second;
                    break;
                case 5:
                    settings_.close_invetory.x = coords.first;
                    settings_.close_invetory.y = coords.second;
                    break;
                default:
                    break;
                }
            }
            ++counter;
            continue;
        }
        
        if (line == "DropMode"s) {
            settings_is_found = true;
            continue;
        }

        if (settings_is_found == true && line == "") {
            input.close();
            return;
        }
    }

    throw std::logic_error("Unknow error the file may be empty"s);
}

void DropMode::EditCoords() {
    Sleep(400);
    std::ifstream input;
    input.open(path_settings_);

    if (!input.is_open()) {
        std::cout << path_settings_ << " file was not created or opened"s << std::endl;
        return;
    }
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    input.close();

    for (std::size_t i = 0; i < 6; i++) {
        
        switch (i) {
        case 0:
            std::cout << "specify the coordinates of the search window (entering the name of the teleport)"s << std::endl;
            GetSetCursorPosition(settings_.search_window.x, settings_.search_window.y);
            break;
        case 1:
            std::cout << "indicate the coordinates of the first teleport in the list"s << std::endl;
            GetSetCursorPosition(settings_.first_tp_name_in_list.x, settings_.first_tp_name_in_list.y);
            break;
        case 2:
            std::cout << "indicate the coordinates of the main teleportation button"s << std::endl;
            GetSetCursorPosition(settings_.teleporting.x, settings_.teleporting.y);
            break;
        case 3:
            std::cout << "indicate the coordinates of the take all button"s << std::endl;
            GetSetCursorPosition(settings_.take_all.x, settings_.take_all.y);
            break;
        case 4:
            std::cout << "indicate the coordinates of the give all button"s << std::endl;
            GetSetCursorPosition(settings_.give_all.x, settings_.give_all.y);
            break;
        case 5:
            std::cout << "indicate the coordinates of the close inventory button"s << std::endl;
            GetSetCursorPosition(settings_.close_invetory.x, settings_.close_invetory.y);
            break;
        }
    }

    lines[16] = "search_window: "s + std::to_string(settings_.search_window.x) + ", "s + std::to_string(settings_.search_window.y);
    lines[17] = "first_tp_name_in_list: "s + std::to_string(settings_.first_tp_name_in_list.x) + ", "s + std::to_string(settings_.first_tp_name_in_list.y);
    lines[18] = "teleporting: "s + std::to_string(settings_.teleporting.x) + ", "s + std::to_string(settings_.teleporting.y);
    lines[19] = "take_all: "s + std::to_string(settings_.take_all.x) + ", "s + std::to_string(settings_.take_all.y);
    lines[20] = "give_all: "s + std::to_string(settings_.give_all.x) + ", "s + std::to_string(settings_.search_window.y);
    lines[21] = "close_inventory: "s + std::to_string(settings_.close_invetory.x) + ", "s + std::to_string(settings_.close_invetory.y);

    std::ofstream out;
    out.open(path_settings_);
    if (!out.is_open()) {
        std::cout << "Couldn't open the file: "s << path_settings_ << std::endl;
        return;
    }
    for (const auto& a : lines) {
        out << a << std::endl;
    }
    out.close();
    mode_selection.first = Command::NONE;
    mode_selection.second = Command::NONE;
    drop_mode_command = DropModeEnum::NONE;
}

void DropMode::EditTpName(){

    Sleep(400);
    std::cout << "the name of the teleport with the safe is indicated last; it does not need to be taken into account in the total number"s << std::endl << std::endl;

    std::cout << "(all letters must be in big case)"s << std::endl;
    std::cout << "example:\nDROP\nDROP1\nDROP 2\nDROP SAVE\netc."s << std::endl << std::endl;

    std::cout << "\n\nindicate the number of teleports (1,2,3,4.....N)"s << std::endl;
    
    int tp_n = 0;
    std::cout << "Enter number: "s;
    std::cin >> tp_n;
    std::vector<std::string> teleport_name;
    teleport_name.reserve(tp_n + 1);
    if (tp_n < 0) {
        tp_n = std::abs(tp_n);
    }
#ifdef max
#undef max
#endif
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Sleep(1000);

    for (int i = 0; i < tp_n; i++) {
        std::cout << i + 1 << " Enter tp name: "s;
        std::string result;
        std::getline(std::cin, result);
        teleport_name.push_back(result);
    }
    std::string save_tp;
    std::cout << "Enter save (vault) tp: "s;
    std::getline(std::cin, save_tp);

    std::ifstream input;
    input.open(path_settings_);
    if (!input.is_open()) {
        std::cout << path_settings_ << " file was not created or opened"s << std::endl;
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    std::string result;
    result += "name of teleports: "s;
    for (std::size_t i = 0; i < teleport_name.size(); i++) {
        if (i != teleport_name.size() - 1) {
            result += teleport_name.at(i);
            result += ", ";
        }
        else {
            result += teleport_name.at(i);
        }
    }

    lines[23] = result;
    lines[24] = "name of teleport for saving loot: "s + save_tp;

    std::ofstream out;
    out.open(path_settings_);
    if (!out.is_open()) {
        std::cout << "Couldn't open the file: "s << path_settings_ << std::endl;
        return;
    }
    for (const auto& a : lines) {
        out << a << std::endl;
    }
    out.close();
    mode_selection.first = Command::NONE;
    mode_selection.second = Command::NONE;
    drop_mode_command = DropModeEnum::NONE;
}

void DropMode::EditSettings(){

    system("cls");

    std::cout << "F1 - Edit coords pos"s << std::endl;
    std::cout << "F2 - Edit drop name"s << std::endl;

    while (mode_selection.first == Command::EDIT_DROP_MODE) {
        if (mode_selection.second == Command::EDIT_DROP_COORD) {
            EditCoords();
            return;
        }
        if (mode_selection.second == Command::EDIT_DROP_NAME) {
            EditTpName();
            return;
        }
        Sleep(100);
    }

}

void DropMode::StartLooting() {

    Sleep(500);
    system("cls");

    std::cout << "F1 - start"s << std::endl;
    std::cout << "F2 - set coords"s << std::endl;
    std::cout << "F3 - set teleport name"s << std::endl;

    int food = 0;

    while (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS || mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::START) {

        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::START) {

            if (teleports_name_.size() == 0) {
                std::cout << "tp_name not found\n\nSleep 10s"s;
                Sleep(10000);
                system("cls");
                std::cout << "F1 - start"s << std::endl;
                std::cout << "F2 - set coords"s << std::endl;
                std::cout << "F3 - set teleport name"s << std::endl;
                break;
            }
            AllocConsole();
            HWND consoleWindow = GetConsoleWindow();
            int width = 200;
            int height = 400;
            SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

            while (true) {

                for (int i = 0; i < teleports_name_.size(); i++) {
                    std::string tp_name = teleports_name_[i];
                    LootingCrete(tp_name, true);
                    Sleep(5000);
                    system("cls");
                    std::cin.clear();
                    std::cout << "Next tp\n";
                }
                Sleep(500);
                LootingCrete(save_teleport_name_, false);
                if (food == 3) {
                    SimulateKeyPress('1');
                    SimulateKeyPress('2');
                    food = -1;
                }
                ++food;
                system("cls");
                std::cout << "Sleep 10 min"s << std::endl;
                Sleep(600000);
                system("cls");
            }
        }
        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS && drop_mode_command == DropModeEnum::EDIT_COORDS) {
            EditCoords();
        }
        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS && drop_mode_command == DropModeEnum::EDIT_TP_NAME) {
            EditTpName();
        }
        Sleep(20);
    }
    system("cls");
    MenuMessage();
}
