#pragma once
#include <filesystem>

#include "image_processing.h"

std::filesystem::path operator""_p(const char* ch, std::size_t size) {
	return std::filesystem::path(ch, ch + size);
}

class DiscordWebhook {

	void SetWebhook();

public:

	DiscordWebhook();
	DiscordWebhook(std::string webhook_url);
	
	void SendImage(int x, int y, int width, int height, const std::filesystem::path& image_path);

	int system_no_output(std::string command) noexcept;

	const std::filesystem::path GetWebHookUrl() const;

	void SendText(const std::filesystem::path& message);

private:

	const std::string cmd_1_for_text_message_ = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"";

	const std::string cmd_2_for_text_message_ = "\\\"}\" ";

	const std::string cmd_for_image_message_ = "curl -F \"file1=@";

	std::filesystem::path webhook_path_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

}; //class DiscordWebhook 
