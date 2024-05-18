#pragma once

#include <filesystem>
#include <string>
#include <utility>
#include <vector>
#include <Windows.h>

namespace utils {
	using namespace std::literals;

	struct Point {
		int x = 0;
		int y = 0;
	};

	enum class Command {
		NONE,
		SLEEP,
		LEFT_CLICK,
		RIGHT_CLICK,
		SPACE_CLICK,
		OPEN_GIT,
		OPEN_YT,
		LOG_MODE,
		DELAY,
		FARM_MODE,
		SELECT_SETTINGS,
		START,
		SPAM_MODE,
		DROP_MODE,
		EDIT_SPAM_MOD,
		EDIT_FARM_MODE,
		EDIT_DROP_MODE,
		EDIT_DROP_COORD,
		EDIT_DROP_NAME,
		EDIT_WEBHOOK,
		EDIT_FARM_COORD,

	}; // class Command

	enum class Resources {
		NONE,
		DELAY,
		FLINT,
		STONE,
		WOOD,
		BERRY,
		THATCH,
		SAND,
		METAL,
	}; // class Resources

	enum class ScreenResolution {
		NONE,
		S_1920x1080,
		S_2560x1440,
		S_2048x1080,
		S_3840x2160,
		FULL_SCREEN,
	}; // class ScreenResolution

	enum class SpamCommand {
		NONE,
		EDIT,
		START,
	}; // class SpamCommand

	enum class Page {
		NONE,
		FIRST,
		SECOND,
	}; // class Page

	enum class DropModeEnum {
		NONE,
		EDIT_COORDS,
		EDIT_TP_NAME,
	}; // class DropModeEnum

	inline std::pair<Command, Command> mode_selection = { Command::NONE, Command::NONE };
	inline bool is_enable = false;
	inline Resources resources = Resources::NONE;
	inline ScreenResolution screen_resolution = ScreenResolution::NONE;
	inline SpamCommand spam_command = SpamCommand::NONE;
	inline Page page_menu = Page::FIRST;
	inline DropModeEnum drop_mode_command = DropModeEnum::NONE;
	
	void CheckFileSettings(const std::filesystem::path& path);
	void EditWebhook(const std::filesystem::path& path);
	void EnableDisableFunc(Command com);
	void GetCursorPosition(bool& a_waiting_key_press, int& x, int& y) noexcept;
	void GetSetCursorPosition(int& x, int& y);

	void Observe();
	void ObserveDropModeSelectSettings();
	void ObserveEditDropModeSettings();

	void ObserveFarmModeSelectSettings();
	void ObserveFarmModeStart();

	void ObserveLogModeSelectSettings();
	void ObserveLogModeStart();

	void ObserveSelectingModeFromTheFirstPage();
	void ObserveSelectingModeFromTheSecondPage();

	void ObserveSpamModeSelectSettings();
	void ObserveSpamModeStart();

	void OpenGitHub() noexcept;
	void OpenYouTube() noexcept;

	std::vector<std::string> ReadSettings(const std::filesystem::path& path_settings);
	void SimulateKeyPress(WORD keyCode);
} // namespace utils