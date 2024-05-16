#include <cmath>
// #include <cstdio>
#include <fstream>
#include <iostream>
#include <utility>

#include "drop_mode.h"
#include "string_processing.h"

namespace farm {
    using namespace std::literals;

    std::filesystem::path operator""_p(const char* ch, std::size_t size) {
        return std::filesystem::path(ch, ch + size);
    }

    DropMode::DropMode(const std::filesystem::path& settings_path) : path_settings_(settings_path) {
        SetSettings();
    }

// ---------------------------------- [Drop Mode] Realization -------------------------
//                                                                                    +
//                                                                                    + ---------------------------
// ------------------------------------------------------------------------------------ Drop Mode Private Methods +

    void DropMode::CloseInventory() {
        Sleep(500);
        std::cout << "Closing inventory, start"s << std::endl;
        SetCursorPos(settings_.close_invetory.x, settings_.close_invetory.y);
        left_button_.Click();
        std::cout << "Closing inventory, end"s << std::endl;
        Sleep(500);
    }

    void DropMode::ClickOnSearchBox() {
        std::cout << "Clicking on the search box, start"s << std::endl;
        SetCursorPos(settings_.search_window.x, settings_.search_window.y);
        Sleep(500);
        left_button_.Click();
        std::cout << "Clicking on the search box, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::ClickToTPName() {
        std::cout << "Clicking on TP name, start"s << std::endl;
        Sleep(500);
        SetCursorPos(settings_.first_tp_name_in_list.x, settings_.first_tp_name_in_list.y);
        Sleep(500);
        left_button_.Click();
        std::cout << "Clicking on TP name, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::ClickToTeleporting() {
        std::cout << "Teleporting, start"s << std::endl;
        SetCursorPos(settings_.teleporting.x, settings_.teleporting.y);
        Sleep(500);
        left_button_.Click();
        std::cout << "Teleporting, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::EditCoords() {
        Sleep(400);

        std::vector<std::string> settings = std::move(ReadSettings(path_settings_));
        SaveSettings();

        settings[16] = "search_window: "s + std::to_string(settings_.search_window.x) + ", "s + std::to_string(settings_.search_window.y);
        settings[17] = "first_tp_name_in_list: "s + std::to_string(settings_.first_tp_name_in_list.x) + ", "s + std::to_string(settings_.first_tp_name_in_list.y);
        settings[18] = "teleporting: "s + std::to_string(settings_.teleporting.x) + ", "s + std::to_string(settings_.teleporting.y);
        settings[19] = "take_everything: "s + std::to_string(settings_.take_everything.x) + ", "s + std::to_string(settings_.take_everything.y);
        settings[20] = "give_everything: "s + std::to_string(settings_.give_everything.x) + ", "s + std::to_string(settings_.give_everything.y);
        settings[21] = "close_inventory: "s + std::to_string(settings_.close_invetory.x) + ", "s + std::to_string(settings_.close_invetory.y);

        std::ofstream out(path_settings_, std::ios::out);

        if (!out.is_open()) {
            std::cout << "The file wasn't opened: "s << path_settings_ << std::endl;
            return;
        }

        for (const auto& setting : settings) {
            out << setting << std::endl;
        }
        out.close();

        mode_selection.first = Command::NONE;
        mode_selection.second = Command::NONE;
        drop_mode_command = DropModeEnum::NONE;
    }

    void DropMode::EditTPName() {

        Sleep(400);
        std::cout << "Safety teleport should be indicated the last; it won't be included into the quantity you will enter."s << std::endl << std::endl;
        std::cout << "NOTE: All letters should be in the UPPER case."s << std::endl;

        std::cout << "Example:\nDROP\nDROP1\nDROP 2\nDROP SAVE\netc."s << std::endl << std::endl;
        std::cout << std::endl << std::endl <<"Indicate the quantity of teleports (1, 2, 3, 4 ..... N)."s << std::endl;

        std::vector<std::string> teleport_name = std::move(RequestUserTPNames());
        std::vector<std::string> settings = std::move(MergeTeleportNames(teleport_name)); // to change

        std::ofstream out(path_settings_, std::ios::out);

        if (!out.is_open()) {
            std::cout << "File wasn't opened: "s << path_settings_ << std::endl;
            return;
        }

        for (const auto& setting : settings) {
            out << setting << std::endl;
        }
        out.close();

        mode_selection.first = Command::NONE;
        mode_selection.second = Command::NONE;
        drop_mode_command = DropModeEnum::NONE;
    }

    void DropMode::EnterTPName(std::string_view teleport_name) {

        std::cout << "Entering TP name, start"s << std::endl;
        for (auto letter : teleport_name) {
            SimulateKeyPress(letter);   // enter tp_name
            Sleep(50);
        }

        std::cout << "Entering TP name, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::GiveEverything() {
        Sleep(500);
        std::cout << "Putting into the vault, start"s << std::endl;
        SetCursorPos(settings_.give_everything.x, settings_.give_everything.y);

        Sleep(200);
        left_button_.Click();
        std::cout << "Putting into the vault, end"s << std::endl;
        Sleep(500);
    }

    void DropMode::LootCrate(std::string_view current_tp_name, bool take) {

        system("cls");
        Sleep(5000);
        MouseDown();

        OpenTP();
        ClickOnSearchBox();
        EnterTPName(current_tp_name);
        ClickToTPName();
        ClickToTeleporting();

        std::cout << "Sleep 10s"s << std::endl;
        Sleep(10000);
        MouseUp();

        MoveTo360();
        if (take == true) {
            TakeEverything();
        }
        if (take == false) {
            GiveEverything();
        }
        CloseInventory();

        std::cout << "Sleep 10s"s << std::endl;
        Sleep(10000);
    }

    std::vector<std::string> DropMode::MergeTeleportNames(std::span<std::string> teleport_names) const {
        std::string save_tp;
        std::cout << "Enter the name of the teleport to save your loot (MY TP SAVE): "s;
        std::getline(std::cin, save_tp);

        std::vector<std::string> settings = std::move(ReadSettings(path_settings_));
        std::string result;
        result += "Name of teleports: "s;

        for (std::size_t i = 0; i < teleport_names.size(); i++) {
            if (i != teleport_names.size() - 1) {
                result += teleport_names[i];
                result += ", ";
            }
            else {
                result += teleport_names[i];
            }
        }

        settings[23] = result;
        settings[24] = "Name of the teleport for saving loot: "s + save_tp;

        return settings;
    }

    void DropMode::MouseDown() const {

        std::cout << "Moving mouse down, start"s << std::endl;
        for (int i = 0; i < 100; i++) {
            mouse_event(MOUSEEVENTF_MOVE, 0, 10, 0, 0);  // full down
        }

        std::cout << "Moving mouse down, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::MouseUp() const {

        std::cout << "Moving mouse up, start"s << std::endl;
        for (int i = 0; i < 4 /* 67 */ ; i++) {
            mouse_event(MOUSEEVENTF_MOVE, 0, -10, 0, 0);
        }

        std::cout << "Moving mouse up, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::MoveTo360() const {

        std::cout << "360° with E, start"s << std::endl;
        for (int i = 0; i < 380; i++) {
            mouse_event(MOUSEEVENTF_MOVE, -10, 0, 0, 0);
            SimulateKeyPress('E');
            Sleep(20);
        }

        std::cout << "360° with E, end"s << std::endl;
        Sleep(200);
    }

    void DropMode::OpenTP() const {
        std::cout << "Opening teleport, start"s << std::endl;
        SimulateKeyPress('E');
        std::cout << "Opening teleport, end"s << std::endl;
        Sleep(1000);
    }

    std::vector<std::string> DropMode::RequestUserTPNames() {
        int tp_num = 0;
        std::cout << "Enter the quantity: "s;
        std::cin >> tp_num;
        std::vector<std::string> teleport_name;
        teleport_name.reserve(tp_num + 1);

        if (tp_num < 0) {
            tp_num = std::abs(tp_num);
        }

#ifdef max
#undef max
#endif

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Sleep(1000);

        for (int i = 0; i < tp_num; i++) {
            std::cout << i + 1 << " Enter TP name: "s;
            std::string result;
            std::getline(std::cin, result);
            teleport_name.push_back(std::move(result));
        }

        return teleport_name;
    }

    void DropMode::SaveSettings() {
        for (std::size_t i = 0; i < 6; i++) {

            switch (i) {
            case 0:
                std::cout << "Specify coordinates of the search window (enter a name of the teleport)."s << std::endl;
                GetSetCursorPosition(settings_.search_window.x, settings_.search_window.y);
                break;

            case 1:
                std::cout << "Indicate coordinates of the first teleport in the list."s << std::endl;
                GetSetCursorPosition(settings_.first_tp_name_in_list.x, settings_.first_tp_name_in_list.y);
                break;

            case 2:
                std::cout << "Indicate coordinates of the main teleportation button."s << std::endl;
                GetSetCursorPosition(settings_.teleporting.x, settings_.teleporting.y);
                break;

            case 3:
                std::cout << "Indicate coordinates of the \"Take everything\" button."s << std::endl;
                GetSetCursorPosition(settings_.take_everything.x, settings_.take_everything.y);
                break;

            case 4:
                std::cout << "Indicate coordinates of the \"Give everything\" button."s << std::endl;
                GetSetCursorPosition(settings_.give_everything.x, settings_.give_everything.y);
                break;

            case 5:
                std::cout << "Indicate coordinates of the \"Close inventory\" button."s << std::endl;
                GetSetCursorPosition(settings_.close_invetory.x, settings_.close_invetory.y);
                break;
            }
        }
    }

    void DropMode::SetSettings() {
        std::ifstream input(path_settings_, std::ios::in);

        if (!input.is_open()) {
            std::cout << "Settings were not saved or a file was not opened: "s << path_settings_ << std::endl;
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
                    auto [x, y] = ParseCoords(line);

                    switch (counter) {
                    case 0:
                        settings_.search_window.x = x;
                        settings_.search_window.y = y;
                        break;

                    case 1:
                        settings_.first_tp_name_in_list.x = x;
                        settings_.first_tp_name_in_list.y = y;
                        break;

                    case 2:
                        settings_.teleporting.x = x;
                        settings_.teleporting.y = y;
                        break;

                    case 3:
                        settings_.take_everything.x = x;
                        settings_.take_everything.y = y;
                        break;

                    case 4:
                        settings_.give_everything.x = x;
                        settings_.give_everything.y = y;
                        break;

                    case 5:
                        settings_.close_invetory.x = x;
                        settings_.close_invetory.y = y;
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

        throw std::logic_error("An unknown error. The file may be empty."s);
    }

    void DropMode::TakeEverything() {
        Sleep(500);
        std::cout << "Taking everything, start"s << std::endl;
        SetCursorPos(settings_.take_everything.x, settings_.take_everything.y);
        Sleep(200);
        left_button_.Click();
        std::cout << "Taking everything, end"s << std::endl;
        Sleep(500);
    }

// ----------------------------------- [Drop Mode] Definition -------------------------
//                                                                                    +
//                                                                                    + ---------------------
// ------------------------------------------------------------------------------------ Drop Mode Interface +

    void DropMode::EditSettings() {
        system("cls");

        std::cout << "F1 - Edit coords pos"s << std::endl;
        std::cout << "F2 - Edit drop name"s << std::endl;

        while (mode_selection.first == Command::EDIT_DROP_MODE) {
            if (mode_selection.second == Command::EDIT_DROP_COORD) {
                EditCoords();
                return;
            }
            if (mode_selection.second == Command::EDIT_DROP_NAME) {
                EditTPName();
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

        while (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS 
            || mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::START) {

            if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::START) {

                if (teleports_name_.size() == 0) {
                    std::cout << "TP name was not found."s;
                    std::cout << std::endl << std::endl << "Sleep 10s"s;

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
                        LootCrate(tp_name, true);
                        Sleep(5000);
                        system("cls");
                        std::cin.clear();
                        std::cout << "Next TP"s << std::endl;
                    }

                    Sleep(500);
                    LootCrate(save_teleport_name_, false);

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
                EditTPName();
            }
            Sleep(20);
        }
        system("cls");
        MenuMessage();
    }
} // namespace farm