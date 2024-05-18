#pragma once
#include "image_processing.h"

namespace webhook {
	std::filesystem::path operator""_p(const char* ch, std::size_t size);

	class DiscordWebhook {
		void SetWebhook();

	public:
		DiscordWebhook();
		DiscordWebhook(std::string webhook_url);
		void CraftCommand(const std::string& message);
		const std::filesystem::path GetWebHookUrl() const;
		int SendDiscordMessage(std::string& command) noexcept;
		void SendImage(int x, int y, int width, int height, const std::filesystem::path& image_path);

	private:
		const std::string cmd_1_for_text_message_ = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"";

		const std::string cmd_2_for_text_message_ = "\\\"}\" ";

		const std::string cmd_for_image_message_ = "curl -F \"file1=@";

		std::filesystem::path webhook_path_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;
	};
} // namespace webhook