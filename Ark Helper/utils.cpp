#include "utils.h"
#include "string_processing.h"

#include <iostream>
#include <fstream>

void OpenYouTube() noexcept{
	LPCWSTR url = L"https://www.youtube.com/channel/UCa4dY-BwibiLpV1JsdHfXDw";
	HINSTANCE result = ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}

void OpenGitHub() noexcept{
	LPCWSTR url = L"https://github.com/prizrak56";
	HINSTANCE result = ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}

void GetCursorPosition(bool& a_waiting_key_press, int& x, int& y) noexcept{

    POINT* pCursorPos = new POINT;
    a_waiting_key_press = true;

    system("cls");
    std::cout << "F1 - Get cursor position\nF2 - Disable\n";
    while (a_waiting_key_press) {
        if (GetAsyncKeyState(VK_F1)) {
            Beep(200, 200);
            if (GetCursorPos(pCursorPos)) {
                x = pCursorPos->x;
                y = pCursorPos->y;
                std::cout << "X: " << x << ", Y: " << y << std::endl;
            }
            else {
                std::cout << "Failed to get cursor position" << std::endl;
            }
            Sleep(50);
        }
        if (GetAsyncKeyState(VK_F2)) {
            a_waiting_key_press = false;
            Beep(200, 250);
            Sleep(300);
            Beep(200, 250);
            system("cls");
            MenuMessage();
        }
        Sleep(20);
    }
}

void EnableDisableFunc(Command com) {
	if (is_enable == false && com != Command::NONE) {
		mode_selection.first = com;
		is_enable = true;
	}
	else {
		mode_selection.first = Command::NONE;
		is_enable = false;
		Beep(6000, 200);
		Beep(6000, 200);
	}
}

void ObServer() {

	while (true) {

		switch (page_menu) {
		case Page::FIRST:
			if (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE && page_menu == Page::FIRST) {
				ObServerSelectingModeFromTheFirstPage();
			}
			if (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
				ObServerLogModeSelectSettings();
			}
			if (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::START) {
				ObServerLogModeStart();
			}
			if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
				ObServerFarmModeSelectSettings();
			}
			if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
				ObServerFarmModeStart();
			}
			if (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
				ObServerSpamModeSelectSettings();
			}
			if (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {
				ObServerSpamModeStart();
			}
			Sleep(100);
			break;

		case Page::SECOND:

			if (page_menu == Page::SECOND && mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {
				ObServerSelectingModeFromTheSecondPage();
			}
			break;
		}
	}
}

void ObServerDropModeSelectSettings(){
	while (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
		if (GetAsyncKeyState(VK_F1)) {
			mode_selection.first = Command::DROP_MODE;
			mode_selection.second = Command::START;
		}
		if (GetAsyncKeyState(VK_F2)) {
			drop_mode_command = DropModeEnum::EDIT_COORDS;
		}
		if (GetAsyncKeyState(VK_F3)) {
			drop_mode_command = DropModeEnum::EDIT_TP_NAME;
		}
		Sleep(100);
	}
}

void ObServerLogModeSelectSettings(){
	while (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::SELECT_SETTINGS) {

		if (GetAsyncKeyState(VK_F1)) {
			Beep(3000, 400);
			screen_resolution = ScreenResolution::S_1920x1080;
		}
		if (GetAsyncKeyState(VK_F2)) {
			Beep(3000, 400);
			screen_resolution = ScreenResolution::S_2560x1440;
		}
		if (GetAsyncKeyState(VK_F3)) {
			Beep(3000, 400);
			screen_resolution = ScreenResolution::S_2048x1080;
		}
		if (GetAsyncKeyState(VK_F4)) {
			Beep(3000, 400);
			screen_resolution = ScreenResolution::S_3840x2160;
		}
		if (GetAsyncKeyState(VK_F5)) {
			Beep(3000, 400);
			screen_resolution = ScreenResolution::FULL_SCREEN;
		}
		Sleep(100);
	}
}

void ObServerLogModeStart(){
	while (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::START) {
		if (GetAsyncKeyState(VK_F2)) {
			mode_selection.second = Command::NONE;
			screen_resolution = ScreenResolution::NONE;
			EnableDisableFunc(Command::NONE);
		}
		Sleep(100);
	}
}

void ObServerFarmModeSelectSettings(){

	while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
		if (GetAsyncKeyState(VK_F1)) {
			mode_selection.second = Command::START;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F2)) {
			resources = Resources::FLINT;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F3)) {
			resources = Resources::STONE;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F4)) {
			resources = Resources::WOOD;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F5)) {
			resources = Resources::BERRY;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F6)) {
			resources = Resources::THATCH;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F7)) {
			resources = Resources::SAND;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F8)) {
			resources = Resources::METAL;
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F9)) {
			resources = Resources::DELAY;
			Sleep(500);
		}
		Sleep(100);
	}

}

void ObServerFarmModeStart(){
	while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
		if (GetAsyncKeyState(VK_F2)) {
			mode_selection.second = Command::NONE;
			EnableDisableFunc(Command::NONE);
		}
		Sleep(100);
	}
}

void ObServerSpamModeSelectSettings(){
	while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
		if (GetAsyncKeyState(VK_F1)) {
			mode_selection.second = Command::START;
		}
		if (GetAsyncKeyState(VK_F2)) {
			spam_command = SpamCommand::EDIT;
		}
		Sleep(100);
	}
}

void ObServerSpamModeStart(){
	while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {
		if (GetAsyncKeyState(VK_F1)) {
			spam_command = SpamCommand::START;
		}

		if (GetAsyncKeyState(VK_F2)) {
			mode_selection.second = Command::NONE;
			spam_command = SpamCommand::NONE;
			EnableDisableFunc(Command::NONE);
		}
	}
}

void ObServerSelectingModeFromTheFirstPage(){

	while (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE && page_menu == Page::FIRST) {
		if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
			Beep(500, 400);
			EnableDisableFunc(Command::LEFT_CLICK);
			Sleep(500);
		}
		if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {
			Beep(1000, 400);
			EnableDisableFunc(Command::RIGHT_CLICK);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F3)) {
			Beep(1500, 400);
			EnableDisableFunc(Command::SPACE_CLICK);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F4)) {
			Beep(2000, 400);
			EnableDisableFunc(Command::OPEN_GIT);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F5)) {
			Beep(2500, 400);
			EnableDisableFunc(Command::OPEN_YT);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F6)) {
			Beep(3000, 400);
			EnableDisableFunc(Command::LOG_MODE);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F7)) {
			Beep(3500, 400);
			EnableDisableFunc(Command::FARM_MODE);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F8)) {
			Beep(4000, 400);
			EnableDisableFunc(Command::DELAY);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F9)) {
			Beep(4500, 400);
			EnableDisableFunc(Command::SPAM_MODE);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {
			page_menu = Page::SECOND;
			system("cls");
			MenuMessage2();
		}
		Sleep(100);
	}

}

void ObServerSelectingModeFromTheSecondPage(){

	while (page_menu == Page::SECOND && mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {

		if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
			page_menu = Page::FIRST;
			system("cls");
			MenuMessage();
		}
		if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
			EnableDisableFunc(Command::DROP_MODE);
			mode_selection.first = Command::DROP_MODE;
			mode_selection.second = Command::SELECT_SETTINGS;
			ObServerDropModeSelectSettings();
		}
		if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {

		}
		Sleep(100);
	}
}

bool FileExists(std::string& path) {
	std::ifstream file(path);
	return file.good();
}

void ClearFile(std::string& path) {
	std::ofstream ofs;
	ofs.open(path, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

void SimulateKeyPress(WORD keyCode) {

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