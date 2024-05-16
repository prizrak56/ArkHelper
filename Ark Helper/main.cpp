#include <filesystem>
#include <iostream>
#include <thread>

#include "clicker.h"
#include "drop_mode.h"
#include "farm_mode.h"
#include "log_mode.h"
#include "server_spam.h"
#include "string_processing.h"
#include "utils.h"

using namespace std::literals;

int main() {
	// throw when cout when the error occurs

	std::filesystem::path settings_path("C:\\"_p / "ArkHelper"_p / "settings.txt"_p);
	CheckFileSettings(settings_path);

	SetConsoleTitle(L"prizrak_p");

	std::thread observer(ObServer);

	const LPCWSTR target_window_Name = L"ArkAscended";
	HWND h_window_handle = FindWindow(NULL, target_window_Name);

	clicker::LeftMouseClicker left_click(h_window_handle);
	clicker::RightMouseClicker right_click(h_window_handle);
	clicker::SpaceButtonClicker space_click(h_window_handle);

	farm::FarmMode farm(h_window_handle);
	LogMode log(settings_path.parent_path());
	ServerSpam spam(settings_path);
	farm::DropMode looting_drop(settings_path);

	size_t delay = 0;

	MenuMessage();

	Page last_page = Page::NONE;

	while (true) {
		
		if (page_menu == Page::FIRST && last_page != Page::FIRST) {
			last_page = Page::FIRST;
			system("cls");
			MenuMessage();
		}
		else if (page_menu == Page::SECOND && last_page != Page::SECOND){
			last_page = Page::SECOND;
			system("cls");
			MenuMessage2();
		}

		switch (mode_selection.first) {

		case Command::LEFT_CLICK:
			if (h_window_handle != NULL) {
				left_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else { 
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case Command::RIGHT_CLICK:
			if (h_window_handle != NULL) {
				right_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else { 
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case Command::SPACE_CLICK:
			if (h_window_handle != NULL) {
				space_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else {
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case Command::OPEN_GIT:
			OpenGitHub();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::OPEN_YT:
			OpenYouTube();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::LOG_MODE:
			log.ChooseOptionsAndStart();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::FARM_MODE:
			farm.FarmStart();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::DELAY:

			system("CLS");
			std::cout << "default - 200 ms"s << std::endl;
			std::cout << "Time delay milliseconds: "s << std::endl;
			std::cin >> delay;
			std::cout << std::endl;
			Beep(1000, 200);
			MenuMessage();
			left_click.SetDelay(delay);
			right_click.SetDelay(delay);
			space_click.SetDelay(delay);
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::SPAM_MODE:
			mode_selection.second = Command::SELECT_SETTINGS;
			spam.SpamStart();
			break;

		case Command::DROP_MODE:
			mode_selection.second = Command::SELECT_SETTINGS;
			looting_drop.StartLooting();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::EDIT_SPAM_MOD :
			spam.GetSetButtonPosition();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			page_menu = Page::FIRST;
			system("cls");
			MenuMessage();
			Sleep(200);
			break;

		case Command::EDIT_DROP_MODE:
			looting_drop.EditSettings();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			page_menu = Page::FIRST;
			system("cls");
			MenuMessage();
			Sleep(200);
			break;

		case Command::EDIT_WEBHOOK:
			system("cls");
			EditWebhook(settings_path);
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			page_menu = Page::FIRST;
			system("cls");
			MenuMessage();
			Sleep(200);
			break;

		case Command::EDIT_FARM_COORD:
			system("cls");
			farm.EditCoords();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			page_menu = Page::FIRST;
			system("cls");
			MenuMessage();
			Sleep(200);
			break;
		}
		
		
		Sleep(20);
	}
}