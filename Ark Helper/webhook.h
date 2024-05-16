#pragma once
#include "image_processing.h"

class DiscordWebhook {

	void SetWebhook();

public:

	DiscordWebhook();
	DiscordWebhook(std::string webhook_url);
	
	void SendImage(int x, int y, int width, int height, const std::string& image_path);

	int system_no_output(std::string command) noexcept;

	std::string GetWebHookUrl() const;

	void SendText(std::string message);

private:

	const std::string cmd_1_for_text_message_ = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"";

	const std::string cmd_2_for_text_message_ = "\\\"}\" ";

	const std::string cmd_for_image_message_ = "curl -F \"file1=@";

	std::string webhook_path_ = "C:\\ArkHelper\\settings.txt";

}; //class DiscordWebhook 