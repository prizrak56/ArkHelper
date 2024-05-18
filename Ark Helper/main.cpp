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

	std::filesystem::path settings_path(std::filesystem::path("C:\\") / std::filesystem::path("ArkHelper") / std::filesystem::path("settings.txt"));
	utils::CheckFileSettings(settings_path);

	SetConsoleTitle(L"prizrak_p");

	std::thread observer(utils::Observe);

	const LPCWSTR target_window_Name = L"ArkAscended";
	HWND h_window_handle = FindWindow(NULL, target_window_Name);

	clicker::LeftMouseClicker left_click(h_window_handle);
	clicker::RightMouseClicker right_click(h_window_handle);
	clicker::SpaceButtonClicker space_click(h_window_handle);

	farm_mode::FarmMode farm(h_window_handle);
	processing::LogMode log(settings_path.parent_path());
	spam::ServerSpam spam(settings_path);
	drop_mode::DropMode looting_drop(settings_path);

	size_t delay = 0;

	processing::PrintMenuMessage1();

	utils::Page last_page = utils::Page::NONE;

	while (true) {
		
		if (utils::page_menu == utils::Page::FIRST && last_page != utils::Page::FIRST) {
			last_page = utils::Page::FIRST;
			system("cls");
			processing::PrintMenuMessage1();
		}
		else if (utils::page_menu == utils::Page::SECOND && last_page != utils::Page::SECOND){
			last_page = utils::Page::SECOND;
			system("cls");
			processing::PrintMenuMessage2();
		}

		switch (utils::mode_selection.first) {

		case utils::Command::LEFT_CLICK:
			if (h_window_handle != NULL) {
				left_click.PressKeyUntilHotKeyIsInputed(utils::is_enable);
			}
			else { 
				utils::EnableDisableFunc(utils::Command::NONE);
				std::cout << std::endl << "Process not found ("s << processing::ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case utils::Command::RIGHT_CLICK:
			if (h_window_handle != NULL) {
				right_click.PressKeyUntilHotKeyIsInputed(utils::is_enable);
			}
			else { 
				utils::EnableDisableFunc(utils::Command::NONE);
				std::cout << std::endl << "Process not found ("s << processing::ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case utils::Command::SPACE_CLICK:
			if (h_window_handle != NULL) {
				space_click.PressKeyUntilHotKeyIsInputed(utils::is_enable);
			}
			else {
				utils::EnableDisableFunc(utils::Command::NONE);
				std::cout << std::endl << "Process not found ("s << processing::ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;

		case utils::Command::OPEN_GIT:
			utils::OpenGitHub();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::OPEN_YT:
			utils::OpenYouTube();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::LOG_MODE:
			log.ChooseOptionsAndStart();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::FARM_MODE:
			farm.StartFarm();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::DELAY:

			system("CLS");
			std::cout << "default - 200 ms"s << std::endl;
			std::cout << "Time delay milliseconds: "s << std::endl;
			std::cin >> delay;
			std::cout << std::endl;
			Beep(1000, 200);
			processing::PrintMenuMessage1();
			left_click.SetDelay(delay);
			right_click.SetDelay(delay);
			space_click.SetDelay(delay);
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::SPAM_MODE:
			utils::mode_selection.second = utils::Command::SELECT_SETTINGS;
			spam.SpamStart();
			break;

		case utils::Command::DROP_MODE:
			utils::mode_selection.second = utils::Command::SELECT_SETTINGS;
			looting_drop.StartLooting();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			break;

		case utils::Command::EDIT_SPAM_MOD :
			spam.GetSetButtonPosition();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			utils::page_menu = utils::Page::FIRST;
			system("cls");
			processing::PrintMenuMessage1();
			Sleep(200);
			break;

		case utils::Command::EDIT_DROP_MODE:
			looting_drop.EditSettings();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			utils::page_menu = utils::Page::FIRST;
			system("cls");
			processing::PrintMenuMessage1();
			Sleep(200);
			break;

		case utils::Command::EDIT_WEBHOOK:
			system("cls");
			utils::EditWebhook(settings_path);
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			utils::page_menu = utils::Page::FIRST;
			system("cls");
			processing::PrintMenuMessage1();
			Sleep(200);
			break;

		case utils::Command::EDIT_FARM_COORD:
			system("cls");
			farm.EditCoords();
			utils::mode_selection.first = utils::Command::NONE;
			utils::mode_selection.second = utils::Command::NONE;
			utils::page_menu = utils::Page::FIRST;
			system("cls");
			processing::PrintMenuMessage1();
			Sleep(200);
			break;
		}
		Sleep(20);
	}
}