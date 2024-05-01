#pragma once
#include <utility>
#include <vector>
#include <string>
#include <windows.h>

#include "clicker.h"

class ServerSpam {
public:

	void SpamStart();

private:
	
	void EnterText();

	void SimulateKeyPress(WORD keyCode);

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

	int server_number = 9068;

	LeftMouseClicker left_button;

	std::pair<int, int> press_to_start = { 930,860 };
	std::pair<int, int> search_server = { 1632,194 };
	std::pair<int, int> refresh_server = { 925,936 };
	std::pair<int, int> join_to_server = { 1707,940 };
	std::pair<int, int> join_to_server_mode = { 351,930 };
	std::pair<int, int> first_server = { 892,327 };
	std::pair<int, int> create_ppl = { 1647,962 };
	std::pair<int, int> server_not_found_back = { 175,877 };
	std::pair<int, int> server_not_found_back2 = { 955,961 };
	std::pair<int, int> exit_to_main_menu = { 953,628 };

	static std::vector<std::wstring> process_names;

	std::vector<std::string> process_names_s;
	bool crash_on = false;

	std::vector<WPARAM> spam_words;   // not for spam || string to wparam < - spam text
	std::vector<std::string> string_to_wparam_words;
	const LPCWSTR name_of_the_game_for_crash_process_ = L"ArkAscended"; //  for search game in search menu windows

};

