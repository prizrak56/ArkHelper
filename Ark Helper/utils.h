#pragma once
#include <utility>
#include <string>

enum class Command{
	NONE,
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

struct Point {
	int x = 0;
	int y = 0;
};

inline std::pair<Command, Command> mode_selection = { Command::NONE, Command::NONE };

inline bool is_enable = false;

inline Resources resources = Resources::NONE;

inline ScreenResolution screen_resolution = ScreenResolution::NONE;

inline SpamCommand spam_command = SpamCommand::NONE;

void OpenYouTube() noexcept;

void OpenGitHub() noexcept;

void GetCursorPosition(bool& a_waiting_key_press, int& x, int& y) noexcept;

void EnableDisableFunc(Command com);

void ObServer();

bool FileExists(std::string& path);

void ClearFile(std::string& path);