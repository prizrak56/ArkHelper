#pragma once
#include <utility>
#include <vector>
#include <string>
#include <windows.h>
#include <filesystem>

#include "clicker.h"
#include "utils.h"

class ServerSpam {
private:

	struct SpamCoordsPosSetting {
		Point press_to_start = { 930,860 };
		Point search_server = { 1632,194 };
		Point refresh_server = { 925,936 };
		Point join_to_server = { 1707,940 };
		Point join_to_server_mode = { 351,930 };
		Point first_server = { 892,327 };
		Point create_ppl = { 1647,962 };
		Point server_not_found_back = { 175,877 };
		Point server_not_found_back2 = { 955,961 };
		Point exit_to_main_menu = { 953,628 };
	};

	void EnterText();

	void SimulateCtrlV();

	void PressToStart();

	void SearchServer();

	void RefreshServer();

	void JoinToServer();

	void CreateToPeople();

	void ServerBackBack();

	void ServerLeave();

	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

	void ConverterWstringToString();

	void FindCrashProcess();

	void OpenSearchMenu();

	void ApplyOrInitializeSettingsFromFile();

	void SetSettings();

public:

	void GetSetButtonPosition();

	ServerSpam() = default;

	ServerSpam(const std::filesystem::path& file_settings_path);

	ServerSpam(const std::filesystem::path& file_settings_path, LPCWSTR lp_process_name, std::string name_of_the_process_to_run);

	void SpamStart();

private:
	
	int server_number_ = 0;

	int start_server_number_ = 0;

	int last_server_number_ = 9200;

	LeftMouseClicker left_button_;

	SpamCoordsPosSetting settings_;

	static std::vector<std::wstring> process_names_;

	bool crash_on_ = false;

	std::string name_of_the_game_to_search_in_case_of_crash_ = "ARK SURVIVAL ASCENDED";

	LPCWSTR name_of_the_game_for_crash_process_ = L"ArkAscended"; //  for search game in search menu windows

	std::filesystem::path file_setting_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

}; // class ServerSpam

