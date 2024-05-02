#include "string_processing.h"
#include "server_spam.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <tlhelp32.h>
#include <locale>
#include <fstream>

ServerSpam::ServerSpam(std::string file_settings_path) : file_setting_(file_settings_path) {}

ServerSpam::ServerSpam(std::string file_settings_path, LPCWSTR lp_process_name, std::string name_of_the_process_to_run) :
    file_setting_(file_settings_path), name_of_the_game_for_crash_process_(lp_process_name), name_of_the_game_to_search_in_case_of_crash_(name_of_the_process_to_run){}

void ServerSpam::SimulateKeyPress(WORD keyCode) {

    INPUT input{};

    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = keyCode;
    input.ki.dwFlags = 0;

    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
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
    Sleep(45000);
}

void ServerSpam::CreateToPeople() {
    Sleep(5000);
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

    std::cout << "Subprocesses of PID " << pid << ":" << std::endl;

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

    if (!FileExists(file_setting_)) {
        GetSetButtonPosition();
        CreateFileWithSettings();
    }

    std::ifstream input;
    input.open(file_setting_);

    if (!input.is_open()) {
        std::cout << "Error open file" << std::endl;
        mode_selection.second = Command::NONE;
        EnableDisableFunc(Command::NONE);
        return;
    }

    std::string value_y;
    std::size_t index =0 ;
    while (std::getline(input, value_y)) {

        const auto pos = value_y.find(" ");
        std::string value_x(value_y.begin(), value_y.begin() + pos);
        value_x = value_y.substr(pos + 1, value_y.length());
        switch (index){ 
        case 0:
            settings_.press_to_start.x = std::stoi(value_x);
            settings_.press_to_start.y = std::stoi(value_y);
            break;
        case 1:
            settings_.search_server.x = std::stoi(value_x);
            settings_.search_server.y = std::stoi(value_y);
            break;
        case 2:
            settings_.refresh_server.x = std::stoi(value_x);
            settings_.refresh_server.y = std::stoi(value_y);
            break;
        case 3:
            settings_.server_not_found_back.x = std::stoi(value_x);
            settings_.server_not_found_back.y = std::stoi(value_y);
            break;
        case 4:
            settings_.server_not_found_back2.x = std::stoi(value_x);
            settings_.server_not_found_back2.y = std::stoi(value_y);
            break;
        case 5:
            settings_.join_to_server.x = std::stoi(value_x);
            settings_.join_to_server.y = std::stoi(value_y);
            break;
        case 6:
            settings_.join_to_server_mode.x = std::stoi(value_x);
            settings_.join_to_server_mode.y = std::stoi(value_y);
            break;
        case 7:
            settings_.first_server.x = std::stoi(value_x);
            settings_.first_server.y = std::stoi(value_y);
            break;
        case 8:
            settings_.create_ppl.x = std::stoi(value_x);
            settings_.create_ppl.y = std::stoi(value_y);
            break;
        case 9:
            settings_.exit_to_main_menu.x = std::stoi(value_x);
            settings_.exit_to_main_menu.y = std::stoi(value_y);
            break;
        }
        ++index;
    }
}

void ServerSpam::CreateFileWithSettings() {

    std::ofstream out;
    out.open(file_setting_);

    ClearFile(file_setting_);

    if (!out.is_open()) {
        std::cout << "Error open file" << std::endl;
        mode_selection.second = Command::NONE;
        EnableDisableFunc(Command::NONE);
        return;
    }
    out << settings_.press_to_start.x << " " << settings_.press_to_start.y << std::endl;

    out << settings_.search_server.x << " " << settings_.search_server.y << std::endl;

    out << settings_.refresh_server.x << " " << settings_.refresh_server.y << std::endl;

    out << settings_.server_not_found_back.x << " " << settings_.server_not_found_back.y << std::endl;

    out << settings_.server_not_found_back2.x << " " << settings_.server_not_found_back2.y << std::endl;

    out << settings_.join_to_server.x << " " << settings_.join_to_server.y << std::endl;

    out << settings_.join_to_server_mode.x << " " << settings_.join_to_server_mode.y << std::endl;

    out << settings_.first_server.x << " " << settings_.first_server.y << std::endl;

    out << settings_.create_ppl.x << " " << settings_.create_ppl.y << std::endl;

    out << settings_.exit_to_main_menu.x << " " << settings_.exit_to_main_menu.y << std::endl;
}

void ServerSpam::GetSetButtonPosition(){

    system("cls");

    std::cout << "You need indicate the coordinate of the main button in the first menu of the upper left part of it" << std::endl;
    GetSetCursorPosition(settings_.press_to_start.x, settings_.press_to_start.y);

    std::cout << "specify the coordinate of the search menu from with server list" << std::endl;
    GetSetCursorPosition(settings_.search_server.x, settings_.search_server.y);

    std::cout << "specify the coordinate of the refresh button" << std::endl;
    GetSetCursorPosition(settings_.refresh_server.x, settings_.refresh_server.y);

    std::cout << "specify the coordinate of the back button from the list of servers" << std::endl;
    GetSetCursorPosition(settings_.server_not_found_back.x, settings_.server_not_found_back.y);

    std::cout << "specify the coordinates of the second back button" << std::endl;
    GetSetCursorPosition(settings_.server_not_found_back2.x, settings_.server_not_found_back2.y);

    std::cout << "indicate the coordinate of the first button to connect in the list of servers" << std::endl;
    GetSetCursorPosition(settings_.join_to_server.x, settings_.join_to_server.y);

    std::cout << "specify the coordinates of the second button will connect (the button on the left side of the screen when opening the window with mods)" << std::endl;
    GetSetCursorPosition(settings_.join_to_server_mode.x, settings_.join_to_server_mode.y);

    std::cout << "indicate the coordinate of the first server in the list" << std::endl;
    GetSetCursorPosition(settings_.first_server.x, settings_.first_server.y);

    std::cout << "indicate the coordinates of the character creation button when logging into the server" << std::endl;
    GetSetCursorPosition(settings_.create_ppl.x, settings_.create_ppl.y);

    std::cout << "specify the coordinates of the exit to main menu button" << std::endl;
    GetSetCursorPosition(settings_.exit_to_main_menu.x, settings_.exit_to_main_menu.y);

    system("cls");
}

void ServerSpam::GetSetCursorPosition(int& x, int& y) {

    POINT* p_cursor_pos = new POINT;
    bool button_pressed = false;

    std::cout << "F1 - Get cursor position" << std::endl;
    while (button_pressed == false) {

        if (GetAsyncKeyState(VK_F1)) {
            Beep(200, 200);
            if (GetCursorPos(p_cursor_pos)) {
                x = p_cursor_pos->x;
                y = p_cursor_pos->y;
                std::cout << "success" << std::endl;
            }
            else {
                std::cout << "Failed to get cursor position" << std::endl;
            }
            button_pressed = true;
            Sleep(300);
        }
    }
    delete p_cursor_pos;
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

    setlocale(LC_ALL, "ru");
    AllocConsole();

    std::cout << "Press F1 - to start" << std::endl;
    std::cout << "Press F2 - to edit settings" << std::endl;

    while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {

        if (spam_command == SpamCommand::EDIT) {
            GetSetButtonPosition();
            CreateFileWithSettings();
            spam_command = SpamCommand::NONE;
        }
        Sleep(100);
    }
    std::cout << "Enter server number to get started: ";
    std::cin >> server_number_;
    std::cout << "Prees F1 - to get start\n";

    HWND console_window = GetConsoleWindow();

    // Установка размеров окна консоли
    int width = 200;
    int height = 400;
    SetWindowPos(console_window, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

    while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {

        while (spam_command == SpamCommand::START) {

            std::cout << "PressToStart();" << std::endl;
            PressToStart();

            std::cout << "SearchServer();" << std::endl;
            Sleep(500);
            SearchServer();
            
            std::cout << "RefreshServer();" << std::endl;
            Sleep(2000);
            RefreshServer();

            std::cout << "JoinToServer();" << std::endl;
            Sleep(2000);
            JoinToServer();

            std::cout << "CreateToPeople();" << std::endl;
            Sleep(4000);
            CreateToPeople();

            std::cout << "EnterText();" << std::endl;
            Sleep(500);
            SimulateCtrlV();

            std::cout << "ServerBackBack();" << std::endl;
            Sleep(500);
            ServerBackBack();

            std::cout << "ServerLeave();" << std::endl;
            Sleep(200);
            ServerLeave();

            std::cout << "FindCrashProcess();" << std::endl;
            Sleep(5000);
            FindCrashProcess();

            if (crash_on_) {
                std::cout << "OpenSearchMenu();" << std::endl;
                OpenSearchMenu();
                Sleep(45000);
                crash_on_ = false;
                process_names_.clear();
            }
            std::cout << "Server number: " << server_number_ << std::endl;

            // max server number
            if (server_number_ >= last_server_number_) {
                server_number_ = 9000;
            }
            Sleep(100);
        }
        Sleep(100);
    }
}