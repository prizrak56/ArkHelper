#include "string_processing.h"
#include "server_spam.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <tlhelp32.h>
#include <locale>

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
    SetCursorPos(press_to_start.first, press_to_start.second);
    left_button.Click();
    Sleep(300);
    left_button.Click();
    Sleep(200);
}

void ServerSpam::SearchServer() {
    SetCursorPos(search_server.first, search_server.second);
    left_button.Click();
    Sleep(300);
    std::string int_to_string = std::to_string(server_number);
    for (int i = 0; i < 4; i++) {
        SimulateKeyPress(int_to_string[i]);
        Sleep(200);
    }
    ++server_number;
}

void ServerSpam::RefreshServer() {
    SetCursorPos(refresh_server.first, refresh_server.second);
    Sleep(20);
    left_button.Click();
    Sleep(5000);
}

void ServerSpam::JoinToServer() {
    SetCursorPos(first_server.first, first_server.second);
    left_button.Click();
    Sleep(5000);
    SetCursorPos(join_to_server.first, join_to_server.second);
    left_button.Click();
    Sleep(5000);
    SetCursorPos(join_to_server_mode.first, join_to_server_mode.second);
    left_button.Click();
    Sleep(15000);
}

void ServerSpam::CreateToPeople() {
    Sleep(5000);
    SetCursorPos(create_ppl.first, create_ppl.second);
    Sleep(200);
    left_button.Click();
    Sleep(1000);
}

void ServerSpam::ServerBackBack() {
    Sleep(200);
    SetCursorPos(server_not_found_back.first, server_not_found_back.second);
    left_button.Click();
    Sleep(200);
    SetCursorPos(server_not_found_back2.first, server_not_found_back2.second);
    left_button.Click();
    Sleep(200);
}

void ServerSpam::ServerLeave() {
    Sleep(200);
    SimulateKeyPress(0x1B);
    Sleep(200);
    SetCursorPos(exit_to_main_menu.first, exit_to_main_menu.second);
    Sleep(200);
    left_button.Click();
    Sleep(5000);
}

void ServerSpam::ConverterWstringToString() {

    for (auto& a : process_names) {
        std::wstring wideStr = a;
        std::string narrowStr(wideStr.begin(), wideStr.end());
        if (narrowStr == "Crash!" || narrowStr == "The UE-ShooterGame Game has crashed and will close") {
            crash_on = true;
            return;
        }
    }
    process_names.clear();
}

std::vector<std::wstring> ServerSpam::process_names;

BOOL CALLBACK ServerSpam::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    // Проверяем, принадлежит ли hwnd приложению с указанным PID
    if (pid == *(DWORD*)lParam) {

        int length = GetWindowTextLength(hwnd);
        if (length > 0) {
            std::vector<wchar_t> buffer(length + 1);
            GetWindowText(hwnd, buffer.data(), length + 1);

            // Выводим название окна (название процесса)
            std::wcout << buffer.data() << std::endl;

            process_names.push_back(buffer.data());

        }
    }
    return TRUE;
}

void ServerSpam::FindCrashProcess() {

    HWND hwnd = FindWindow(NULL, name_of_the_game_for_crash_process_);
    if (hwnd == NULL) {
        crash_on = true;
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

    Sleep(500);
    Sleep(1000);
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
    return;
}

void ServerSpam::ApplyOrInitializeSettingsFromFile(){



}

void ServerSpam::EnterText() {
    Sleep(700);
    for (auto x : spam_words) {
        SimulateKeyPress(x);
        Sleep(300);
    }
    Sleep(500);
    SimulateKeyPress(0x0D);
    Sleep(500);
}

void ServerSpam::SpamStart() {
    setlocale(LC_ALL, "ru");
    AllocConsole();

    std::cout << "Enter server number to get started: ";
    std::cin >> server_number;
    std::cout << "Prees F1 - to get start\n";

    HWND consoleWindow = GetConsoleWindow();

    // Установка размеров окна консоли
    int width = 200;
    int height = 400;
    SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

    while (true) {
        if (GetAsyncKeyState(VK_F1)) {
            while (true) {

                std::cout << "PressToStart();" << std::endl;
                PressToStart();

                std::cout << "SearchServer();" << std::endl;
                Sleep(200);
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

                if (crash_on) {
                    std::cout << "OpenSearchMenu();" << std::endl;
                    OpenSearchMenu();
                    Sleep(45000);
                    crash_on = false;
                    process_names.clear();
                    process_names_s.clear();
                }
                std::cout << "Server number: " << server_number << std::endl;

                // max server number
                if (server_number >= 9172) {
                    server_number = 9000;
                }
            }
        }
        Sleep(20);
    }
}