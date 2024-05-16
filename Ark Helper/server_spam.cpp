#include "string_processing.h"
#include "server_spam.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <tlhelp32.h>
#include <locale>
#include <fstream>

using namespace std::literals;

ServerSpam::ServerSpam(const std::filesystem::path& file_settings_path) : file_setting_(file_settings_path) {}

ServerSpam::ServerSpam(const std::filesystem::path& file_settings_path, LPCWSTR lp_process_name, std::string name_of_the_process_to_run) :
    file_setting_(file_settings_path), name_of_the_game_for_crash_process_(lp_process_name), name_of_the_game_to_search_in_case_of_crash_(name_of_the_process_to_run){}

void ServerSpam::SetSettings(){

    std::ifstream input;
    input.open(file_setting_);
    if (!input.is_open()) {
        std::cout << file_setting_ << " file was not created or opened"s << std::endl;
        return;
    }

    std::string line;
    bool settings_is_found = false;

    size_t counter = 0;

    while (std::getline(input, line)) {

        if (settings_is_found == true && line != "") {

            std::pair<int, int> coords = ParseCoords(line);

            switch (counter)
            {
            case 0:
                settings_.press_to_start.x = coords.first;
                settings_.press_to_start.y = coords.second;
                break;
            case 1:
                settings_.search_server.x = coords.first;
                settings_.search_server.y = coords.second;
                break;
            case 2:
                settings_.refresh_server.x = coords.first;
                settings_.refresh_server.y = coords.second;
                break;
            case 3:
                settings_.join_to_server.x = coords.first;
                settings_.join_to_server.y = coords.second;
                break;
            case 4:
                settings_.join_to_server_mode.x = coords.first;
                settings_.join_to_server_mode.y = coords.second;
                break;
            case 5:
                settings_.first_server.x = coords.first;
                settings_.first_server.y = coords.second;
                break;
            case 6:
                settings_.create_ppl.x = coords.first;
                settings_.create_ppl.y = coords.second;
                break;
            case 7:
                settings_.server_not_found_back.x = coords.first;
                settings_.server_not_found_back.y = coords.second;
                break;
            case 8:
                settings_.server_not_found_back2.x = coords.first;
                settings_.server_not_found_back2.y = coords.second;
                break;
            case 9:
                settings_.exit_to_main_menu.x = coords.first;
                settings_.exit_to_main_menu.y = coords.second;
                break;
            }
            ++counter;
            continue;
        }

        if (line == "ServerSpam"s) {
            settings_is_found = true;
            continue;
        }

        if (settings_is_found == true && line == "") {
            input.close();
            return;
        }
    }
}

void ServerSpam::SimulateCtrlV() {
    Sleep(500);
    SimulateKeyPress(0x0D);
    Sleep(1000);
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('V', 0, 0, 0);
    keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    Sleep(1000);
    SimulateKeyPress(0x0D);
    Sleep(1000);
}

void ServerSpam::PressToStart() {
    SetCursorPos(settings_.press_to_start.x, settings_.press_to_start.y);
    left_button_.Click();
    Sleep(300);
    left_button_.Click();
    Sleep(200);
}

void ServerSpam::SearchServer() {
    SetCursorPos(settings_.search_server.x, settings_.search_server.y);
    left_button_.Click();
    Sleep(300);
    std::string int_to_string = std::to_string(server_number_);
    for (int i = 0; i < 4; i++) {
        SimulateKeyPress(int_to_string[i]);
        Sleep(200);
    }
    ++server_number_;
}

void ServerSpam::RefreshServer() {
    SetCursorPos(settings_.refresh_server.x, settings_.refresh_server.y);
    Sleep(100);
    left_button_.Click();
    Sleep(5000);
}

void ServerSpam::JoinToServer() {
    SetCursorPos(settings_.first_server.x, settings_.first_server.y);
    left_button_.Click();
    Sleep(5000);
    SetCursorPos(settings_.join_to_server.x, settings_.join_to_server.y);
    left_button_.Click();
    Sleep(5000);
    SetCursorPos(settings_.join_to_server_mode.x, settings_.join_to_server_mode.y);
    left_button_.Click();
    Sleep(30000);
}

void ServerSpam::CreateToPeople() {
    Sleep(1000);
    SetCursorPos(settings_.create_ppl.x, settings_.create_ppl.y);
    Sleep(200);
    left_button_.Click();
    Sleep(1000);
}

void ServerSpam::ServerBackBack() {
    Sleep(200);
    SetCursorPos(settings_.server_not_found_back.x, settings_.server_not_found_back.y);
    left_button_.Click();
    Sleep(200);
    SetCursorPos(settings_.server_not_found_back2.x, settings_.server_not_found_back2.y);
    left_button_.Click();
    Sleep(200);
}

void ServerSpam::ServerLeave() {
    Sleep(200);
    SimulateKeyPress(0x1B);
    Sleep(200);
    SetCursorPos(settings_.exit_to_main_menu.x, settings_.exit_to_main_menu.y);
    Sleep(200);
    left_button_.Click();
    Sleep(5000);
}

void ServerSpam::ConverterWstringToString() {

    for (auto& a : process_names_) {
        std::wstring wide_str = a;
        std::string narrow_str(wide_str.begin(), wide_str.end());
        if (narrow_str == "Crash!" || narrow_str == "The UE-ShooterGame Game has crashed and will close") {
            crash_on_ = true;
            return;
        }
    }
    process_names_.clear();
}

std::vector<std::wstring> ServerSpam::process_names_;

BOOL CALLBACK ServerSpam::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    // Проверяем, принадлежит ли hwnd приложению с указанным PID
    if (pid == *(DWORD*)lParam) {
        int length = GetWindowTextLength(hwnd);
        if (length > 0) {
            std::vector<wchar_t> buffer(length + 1);
            GetWindowText(hwnd, buffer.data(), length + 1);

            std::wcout << buffer.data() << std::endl;

            process_names_.push_back(buffer.data());
        }
    }
    return TRUE;
}

void ServerSpam::FindCrashProcess() {

    HWND hwnd = FindWindow(NULL, name_of_the_game_for_crash_process_);
    if (hwnd == NULL) {
        crash_on_ = true;
        return;
    }
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    std::cout << "Subprocesses of PID "s << pid << ":" << std::endl;

    // Передаём PID приложения в качестве параметра для EnumWindowsProc
    EnumWindows(EnumWindowsProc, (LPARAM)&pid);

    ConverterWstringToString();
}

void ServerSpam::OpenSearchMenu() {

    Sleep(1500);
    SimulateKeyPress(0x0D);
    Sleep(1000);
    SimulateKeyPress(0x0D);
    Sleep(500);
    keybd_event(VK_LWIN, 0, 0, 0);
    Sleep(100);
    keybd_event('S', 0, 0, 0);
    Sleep(100);
    keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);

    EnterText();
}

void ServerSpam::ApplyOrInitializeSettingsFromFile() {

    if (std::filesystem::exists(file_setting_)) {
        GetSetButtonPosition();
    }
    SetSettings();
}

void ServerSpam::GetSetButtonPosition() {

    system("cls");

    std::cout << "You need indicate the coordinate of the main join button in the first menu of the upper left part of it (Press to start)"s << std::endl;
    GetSetCursorPosition(settings_.press_to_start.x, settings_.press_to_start.y);

    std::cout << "specify the coordinate of the search menu from with server list"s << std::endl;
    GetSetCursorPosition(settings_.search_server.x, settings_.search_server.y);

    std::cout << "specify the coordinate of the refresh button"s << std::endl;
    GetSetCursorPosition(settings_.refresh_server.x, settings_.refresh_server.y);

    std::cout << "specify the coordinate of the back button from the list of servers"s << std::endl;
    GetSetCursorPosition(settings_.server_not_found_back.x, settings_.server_not_found_back.y);

    std::cout << "specify the coordinates of the second back button"s << std::endl;
    GetSetCursorPosition(settings_.server_not_found_back2.x, settings_.server_not_found_back2.y);

    std::cout << "indicate the coordinate of the first button to connect in the list of servers"s << std::endl;
    GetSetCursorPosition(settings_.join_to_server.x, settings_.join_to_server.y);

    std::cout << "specify the coordinates of the second button will connect (the button on the left side of the screen when opening the window with mods)"s << std::endl;
    GetSetCursorPosition(settings_.join_to_server_mode.x, settings_.join_to_server_mode.y);

    std::cout << "indicate the coordinate of the first server in the list"s << std::endl;
    GetSetCursorPosition(settings_.first_server.x, settings_.first_server.y);

    std::cout << "indicate the coordinates of the character creation button when logging into the server"s << std::endl;
    GetSetCursorPosition(settings_.create_ppl.x, settings_.create_ppl.y);

    std::cout << "specify the coordinates of the exit to main menu button"s << std::endl;
    GetSetCursorPosition(settings_.exit_to_main_menu.x, settings_.exit_to_main_menu.y);

    std::ifstream input;

    input.open(file_setting_);

    if (!input.is_open()) {
        std::cout << "settings were not saved file could not open file"s << std::endl;
        return;
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(input,line)){
        lines.push_back(line);
    }
    input.close();

    lines[4] = "pess_to_start: "s + std::to_string(settings_.press_to_start.x) + ", " + std::to_string(settings_.press_to_start.y);
    lines[5] = "search_server: "s + std::to_string(settings_.search_server.x) + ", " + std::to_string(settings_.search_server.y);
    lines[6] = "refresh_server: "s + std::to_string(settings_.refresh_server.x) + ", " + std::to_string(settings_.refresh_server.y);
    lines[7] = "join_to_server: "s + std::to_string(settings_.join_to_server.x) + ", " + std::to_string(settings_.join_to_server.y);
    lines[8] = "join_to_server_mode: "s + std::to_string(settings_.join_to_server_mode.x) + ", " + std::to_string(settings_.join_to_server_mode.y);
    lines[9] = "first_server: "s + std::to_string(settings_.first_server.x) + ", " + std::to_string(settings_.first_server.y);
    lines[10] = "create_people: "s + std::to_string(settings_.create_ppl.x) + ", " + std::to_string(settings_.create_ppl.y);
    lines[11] = "server_not_found_back: "s + std::to_string(settings_.server_not_found_back.x) + ", " + std::to_string(settings_.server_not_found_back.y);
    lines[12] = "server_not_found_back2: "s + std::to_string(settings_.server_not_found_back2.x) + ", " + std::to_string(settings_.server_not_found_back2.y);
    lines[13] = "exit_to_main_menu: "s + std::to_string(settings_.exit_to_main_menu.x) + ", " + std::to_string(settings_.exit_to_main_menu.y);

    std::ofstream out;
    out.open(file_setting_);

    if (!out.is_open()) {
        std::cout << "settings were not saved file could not open file"s << std::endl;
        return;
    }

    for (const auto& str : lines) {
        out << str << std::endl;
    }

    out.close();

    system("cls");
}

void ServerSpam::EnterText() {
    Sleep(700);
    for (auto x : name_of_the_game_to_search_in_case_of_crash_) {
        SimulateKeyPress(x);
        Sleep(300);
    }
    Sleep(500);
    SimulateKeyPress(0x0D);
    Sleep(500);
}

void ServerSpam::SpamStart() {

    ApplyOrInitializeSettingsFromFile();

    system("cls");

    AllocConsole();

    std::cout << "Press F1 - to start"s << std::endl;
    std::cout << "Press F2 - to edit settings"s << std::endl;

    while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {

        if (spam_command == SpamCommand::EDIT) {
            GetSetButtonPosition();
            spam_command = SpamCommand::NONE;
        }
        Sleep(100);
    }
    std::cout << "Enter server number to get started: "s;
    std::cin >> server_number_;
    start_server_number_ = server_number_;
    last_server_number_ = server_number_ + 200;
    std::cout << "Prees F1 - to get start"s << std::endl;

    HWND console_window = GetConsoleWindow();

    // Setting Console Window Sizes
    int width = 200;
    int height = 400;
    SetWindowPos(console_window, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

    while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {

        while (spam_command == SpamCommand::START) {

            std::cout << "PressToStart();"s << std::endl;
            PressToStart();

            std::cout << "SearchServer();"s << std::endl;
            Sleep(500);
            SearchServer();
            
            std::cout << "RefreshServer();"s << std::endl;
            Sleep(2000);
            RefreshServer();

            std::cout << "JoinToServer();"s << std::endl;
            Sleep(2000);
            JoinToServer();

            std::cout << "CreateToPeople();"s << std::endl;
            Sleep(2000);
            CreateToPeople();

            std::cout << "EnterText();"s << std::endl;
            Sleep(500);
            SimulateCtrlV();

            std::cout << "ServerBackBack();"s << std::endl;
            Sleep(500);
            ServerBackBack();

            std::cout << "ServerLeave();"s << std::endl;
            Sleep(200);
            ServerLeave();

            std::cout << "FindCrashProcess();"s << std::endl;
            Sleep(5000);
            FindCrashProcess();

            if (crash_on_) {
                std::cout << "OpenSearchMenu();"s << std::endl;
                OpenSearchMenu();
                Sleep(45000);
                crash_on_ = false;
                process_names_.clear();
            }
            std::cout << "Server number: "s << server_number_ << std::endl;

            // max server number
            if (server_number_ >= last_server_number_) {
                server_number_ = start_server_number_;
            }
            Sleep(100);
        }
        Sleep(100);
    }
}