#include "drop_mode.h"
#include "string_processing.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

void DropMode::MouseDown() {
    std::cout << "Mouse down start\n";
    for (int i = 0; i < 100; i++) {
        mouse_event(MOUSEEVENTF_MOVE, 0, 10, 0, 0);  //full down
    }
    std::cout << "Mouse down end\n";
    Sleep(200);
}

void DropMode::OpenTp() {
    std::cout << "Open teleport start\n";
    SimulateKeyPress('E');
    std::cout << "Open teleport end\n";
    Sleep(1000);
}

void DropMode::SearchClick(Point search_window) {
    std::cout << "Search click start\n";
    SetCursorPos(search_window.x, search_window.y);
    Sleep(500);
    left_button_.Click();
    std::cout << "Search click end\n";
    Sleep(200);
}

void DropMode::EnterTpName(std::string& teleport_name) {

    std::cout << "Enter tp name start\n";
    for (auto a : teleport_name) {
        SimulateKeyPress(a);   //enter tp_name
        Sleep(50);
    }
    std::cout << "Enter tp name end\n";
    Sleep(200);
}

void DropMode::ClickToTpName(Point tp_name_s) {
    std::cout << "Click to tp name start\n";
    Sleep(500);
    SetCursorPos(tp_name_s.x, tp_name_s.y);
    Sleep(500);
    left_button_.Click();
    std::cout << "Click to tp name end\n";
    Sleep(200);
}

void DropMode::ClickToTeleporting(Point teleporting) {
    std::cout << "teleporting start\n";
    SetCursorPos(teleporting.x, teleporting.y); //teleport
    Sleep(500);
    left_button_.Click();
    std::cout << "teleporting end\n";
    Sleep(200);
}

void DropMode::MouseUp() {

    std::cout << "Mouse up start\n";
    for (int i = 0; i < 67; i++) {
        mouse_event(MOUSEEVENTF_MOVE, 0, -10, 0, 0);
    }
    std::cout << "Mouse up end\n";
    Sleep(200);
}

void DropMode::MoveTo360() {

    std::cout << "360 with E start\n";
    for (int i = 0; i < 380; i++) {
        mouse_event(MOUSEEVENTF_MOVE, -10, 0, 0, 0);
        SimulateKeyPress('E');
        Sleep(20);
    }
    std::cout << "360 with E end\n";
    Sleep(200);
}

void DropMode::TakeAll(Point take_all) {
    std::cout << "Take all start\n";
    SetCursorPos(take_all.x, take_all.y);   // take everything
    //mouse_click();
    left_button_.Click();
    std::cout << "Take all end\n";
    Sleep(500);
}

void DropMode::GiveAll(Point give_all) {
    std::cout << "Drop in vault start\n";
    SetCursorPos(give_all.x, give_all.y);    // put everything
    left_button_.Click();
    std::cout << "Drop in vault end\n";
    Sleep(500);
}

void DropMode::CloseInventory(Point close_invetory) {
    std::cout << "Close inventory start\n";
    SetCursorPos(close_invetory.x, close_invetory.y);
    left_button_.Click();
    std::cout << "Close inventory end\n";
    Sleep(500);
}

void DropMode::LootingCrete(std::string& teleport_name, bool take, Point search_window, Point tp_name_s, Point teleporting, Point take_all, Point give_all, Point close_invetory) {

    system("cls");
    Sleep(5000);
    MouseDown();

    OpenTp();

    SearchClick(search_window);

    EnterTpName(teleport_name);

    ClickToTpName(tp_name_s);

    ClickToTeleporting(teleporting);

    std::cout << "Sleep 10s\n";
    Sleep(10000);

    MouseUp();

    MoveTo360();
    if (take == true) {
        TakeAll(take_all);
    }
    if (take == false) {
        GiveAll(give_all);
    }
    CloseInventory(close_invetory);

    std::cout << "Sleep 10s\n";
    Sleep(10000);
}

Point DropMode::ParseButtonCoords(const std::string& input) {
    Point result;
    std::istringstream iss(input);
    iss >> result.x >> result.y;
    return result;
}

void DropMode::GetCursorPositionDrop(Point& coords) noexcept {

    POINT* pCursorPos = new POINT;
    bool customcoords = true;
    std::cout << "F1 - Get cursor position\nF2 - Disable\n";
    while (customcoords) {
        if (GetAsyncKeyState(VK_F1)) {
            if (GetCursorPos(pCursorPos)) {
                coords.x = pCursorPos->x;
                coords.y = pCursorPos->y;
                std::cout << "X: " << coords.x << ", Y: " << coords.y << std::endl;
                return;
            }
        }
        Sleep(20);
    }
}

void DropMode::SupplyCreate() {

    Sleep(500);
    system("cls");

    Point search_window;
    search_window.x = 391;
    search_window.y = 965;

    Point tp_name_s;
    tp_name_s.x = 241;
    tp_name_s.y = 218;

    Point teleporting;
    teleporting.x = 1656;
    teleporting.y = 964;

    Point take_all;
    take_all.x = 1379;
    take_all.y = 199;

    Point give_all;
    give_all.x = 358;
    give_all.y = 196;

    Point close_invetory;
    close_invetory.x = 1798;
    close_invetory.y = 65;

    std::vector<WPARAM> spam_words;
    std::vector<std::string> teleport_name; //= { "DROP1", "DROP2" };
    std::string vault_tp = "DROP LOOT";

    {
        std::ifstream file_coords;
        
        if (FileExists(path_to_file_with_coords_)) {

            file_coords.open(path_to_file_with_coords_);
            std::string line;
            int number_line = 0;
            while (std::getline(file_coords, line)) {

                if (number_line == 0) {
                    search_window = ParseButtonCoords(line);
                }
                else if (number_line == 1) {
                    tp_name_s = ParseButtonCoords(line);
                }
                else if (number_line == 2) {
                    teleporting = ParseButtonCoords(line);
                }
                else if (number_line == 3) {
                    take_all = ParseButtonCoords(line);
                }
                else if (number_line == 4) {
                    give_all = ParseButtonCoords(line);
                }
                else if (number_line == 5) {
                    close_invetory = ParseButtonCoords(line);
                }
                ++number_line;
            }
            file_coords.close();
        }
        else {
            std::cout << "File '" << path_to_file_with_coords_ << "' does not exist.\n" << "Set the coordinates (F2) or the standard ones will be used(1920x1080)\n\n\n";
        }
    }

    {
        std::ifstream file_name;

        if (FileExists(path_to_the_file_with_the_names_of_teleports_)) {

            file_name.open(path_to_the_file_with_the_names_of_teleports_);

            std::string line;
            while (std::getline(file_name, line)) {
                teleport_name.push_back(line);
            }
            std::ifstream vault_name;
            vault_name.open(path_to_the_file_with_the_name_of_save_teleports_);
            std::getline(vault_name, vault_tp);
            vault_name.close();
            file_name.close();
        }
        else {
            std::cout << "File '" << path_to_the_file_with_the_names_of_teleports_ << "' does not exist.\n(Set tp name(F3))\n\n\n";
        }
    }
    std::cout << "F1 - start\n";
    std::cout << "F2 - set coords\n";
    std::cout << "F3 - set teleport name\n";

    int food = 0;

    while (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS) {

        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::START) {

            if (teleport_name.size() == 0) {
                std::cout << "tp_name not found\n\nSleep 10s";
                Sleep(10000);
                system("cls");
                std::cout << "F1 - start\n";
                std::cout << "F2 - set coords\n";
                std::cout << "F3 - set teleport name\n";
                break;
            }
            AllocConsole();
            HWND consoleWindow = GetConsoleWindow();
            int width = 200;
            int height = 400;
            SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

            while (true) {

                for (int i = 0; i < teleport_name.size(); i++) {
                    std::string tp_name = teleport_name[i];
                    LootingCrete(tp_name, true, search_window, tp_name_s, teleporting, take_all, give_all, close_invetory);
                    Sleep(5000);
                    system("cls");
                    std::cin.clear();
                    std::cout << "Next tp\n";
                }
                Sleep(500);
                LootingCrete(vault_tp, false, search_window, tp_name_s, teleporting, take_all, give_all, close_invetory);
                if (food == 3) {
                    SimulateKeyPress('1');
                    SimulateKeyPress('2');
                    food = -1;
                }
                ++food;
                system("cls");
                std::cout << "Sleep 10 min\n";
                Sleep(600000);
                system("cls");
            }
        }
        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS && drop_mode_command == DropModeEnum::EDIT_COORDS) {
            system("cls");
            for (int i = 0; i < 6; i++) {
                if (i == 0) {
                    std::cout << "Set Search window (bottom left of screen(search))\n";
                    GetCursorPositionDrop(search_window);
                    Sleep(500);
                }
                else if (i == 1) {
                    std::cout << "indicate the coordinates of the first teleport in the list\n";
                    GetCursorPositionDrop(tp_name_s);
                    Sleep(500);
                }
                else if (i == 2) {
                    std::cout << "Coordinates of the main button for teleportation\n";
                    GetCursorPositionDrop(teleporting);
                    Sleep(500);
                }
                else if (i == 3) {
                    std::cout << "Coordinates of the transfer all button (NOT your inventory)\n";
                    GetCursorPositionDrop(take_all);
                    Sleep(500);
                }
                else if (i == 4) {
                    std::cout << "Coordinates of the transfer all button (your inventory)\n";
                    GetCursorPositionDrop(give_all);
                    Sleep(500);
                }
                else if (i == 5) {
                    std::cout << "Coordinates of the button to close the inventory (upper right corner)\n";
                    GetCursorPositionDrop(close_invetory);
                    Sleep(500);
                }
                system("cls");
                drop_mode_command = DropModeEnum::NONE;
            }
            std::ofstream out;
            if (out.is_open()) {
                out << search_window.x << " " << search_window.y << std::endl;
                out << tp_name_s.x << " " << tp_name_s.y << std::endl;
                out << teleporting.x << " " << teleporting.y << std::endl;
                out << take_all.x << " " << take_all.y << std::endl;
                out << give_all.x << " " << give_all.y << std::endl;
                out << close_invetory.x << " " << close_invetory.y << std::endl;
            }
            else {
                std::cerr << "No" << std::endl;
            }
            out.close();
            MenuMessage();
            return;
        }
        if (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS && drop_mode_command == DropModeEnum::EDIT_TP_NAME) {
            teleport_name.clear();

            ClearFile(path_to_the_file_with_the_names_of_teleports_);
            ClearFile(path_to_the_file_with_the_name_of_save_teleports_);

            system("cls");
            std::cout << "the name of the teleport with the safe is indicated last; it does not need to be taken into account in the total number\n\n";

            std::cout << "(all letters must be in big case)\nexample:\nDROP\nDROP1\nDROP 2\nDROP SAVE\netc.\n\n";
            std::cout << "\n\nindicate the number of teleports (1,2,3,4.....N)\n";
            int tp_n = 0;
            std::cout << "Enter number: ";
            std::cin >> tp_n;
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
                std::cout << i + 1 << " Enter tp name: ";
                std::string result;
                std::getline(std::cin, result);
                teleport_name.push_back(result);
            }

            std::cout << "Enter vault tp: ";
            std::getline(std::cin, vault_tp);

            std::ofstream out;
            out.open(path_to_the_file_with_the_names_of_teleports_);
            for (int i = 0; i < teleport_name.size(); i++) {
                out << teleport_name[i] << "\n";
            }
            out.close();

            out.open(path_to_the_file_with_the_name_of_save_teleports_);
            out << vault_tp;

            out.close();
            system("cls");
            MenuMessage();
            drop_mode_command = DropModeEnum::NONE;
            return;
        }
        Sleep(20);
    }
    system("cls");
    MenuMessage();
}
