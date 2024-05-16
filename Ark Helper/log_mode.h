#pragma once
#include <string>
#include <chrono>
#include <filesystem>
#include "webhook.h"

class LogMode {

	struct CoordsScreenResolution {
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int width_full = GetSystemMetrics(SM_CXSCREEN);
		int height_full = GetSystemMetrics(SM_CYSCREEN);
	};

	void EditScreenSettings(int x, int y, int width, int height);

	void NoticeUser(int& everyone_time_counter, int& not_everyone_time_counter);

	void SendDiscordMessageAndRestartTimerCustomPosScreenResolution(DiscordWebhook& discord_webhook, std::chrono::steady_clock::time_point& reset_timer_start, int& reset_timer_duration, bool is_everyone);

public:

	LogMode() = default;

	LogMode(const std::filesystem::path& image_save_path);

	void ChooseOptionsAndStart();

private:

	std::filesystem::path image_save_path_ = "C:\\"_p / "ArkHelper"_p / "log.png"_p;

	CoordsScreenResolution screen_coords_;

}; // class LogMode