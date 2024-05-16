#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>
#include <Windows.h>

#include "clicker.h"
#include "utils.h"

namespace spam {
// -------------------------------- [Server Spam] Definition --------------------------
//                                                                                    +
//                                                                                    + --------------------
// ------------------------------------------------------------------------------------ Server Spam Itself +

	std::filesystem::path operator""_p(const char* ch, std::size_t size);

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
			Point back_if_server_not_found = { 175,877 };
			Point back2_if_server_not_found = { 955,961 };
			Point exit_to_main_menu = { 953,628 };
		};

		void ApplyOrInitializeSettingsFromFile();
		void ConverteWstringToString();
		void CreateCharacter();
		void EnterText();
		void FindCrashProcess();
		void JoinToServer();
		void OpenSearchMenu();
		void PressToStart();
		void RefreshServer();

		void SearchServer();
		void BackBackServer();
		void LeaveServer();
		void SetSettings();
		void SimulateCtrlV();

		static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

	public:
		ServerSpam() = default;
		ServerSpam(const std::filesystem::path& file_settings_path);
		ServerSpam(const std::filesystem::path& file_settings_path, LPCWSTR lp_process_name, std::string name_of_the_process_to_run);

		void GetSetButtonPosition();
		void SpamStart();

	private:
		int server_number_ = 0;
		int start_server_number_ = 0;
		int last_server_number_ = 9200;
		bool crash_on_ = false;

		clicker::LeftMouseClicker left_button_;
		SpamCoordsPosSetting settings_;
		std::string game_name_to_search_in_case_of_crash_ = "ARK SURVIVAL ASCENDED";
		LPCWSTR game_name_for_seeking_crash_process_ = L"ArkAscended"; // For seeking the game in the search menu of Windows
		std::filesystem::path file_settings_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

		static std::vector<std::wstring> process_names_;
	};
} // namespace spam