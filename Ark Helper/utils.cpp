#include "utils.h"
#include <windows.h>
#include "string_processing.h"
#include <iostream>

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
            InfoMessage();
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

		while (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {
			if (GetAsyncKeyState(VK_F1)) {
				Beep(500, 400);
				EnableDisableFunc(Command::LEFT_CLICK);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F2)) {
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
			Sleep(100);
		}
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
		while (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::START) {
			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::NONE;
				screen_resolution = ScreenResolution::NONE;
				EnableDisableFunc(Command::NONE);
			}
			Sleep(100);
		}

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
		while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::NONE;
				EnableDisableFunc(Command::NONE);
			}
			Sleep(100);
		}
		Sleep(100);
	}
}