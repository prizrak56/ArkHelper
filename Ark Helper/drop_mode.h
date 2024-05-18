#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "clicker.h"
#include "utils.h"

namespace drop_mode {
	std::filesystem::path operator""_p(const char* ch, std::size_t size);

	class DropMode {
	private:
		struct DropCoordsPosSettings {
			utils::Point search_window;
			utils::Point first_tp_name_in_list;
			utils::Point teleporting;
			utils::Point take_everything;
			utils::Point give_everything;
			utils::Point close_invetory;
		};

		void CloseInventory();
		void ClickOnSearchBox();
		void ClickToTPName();
		void ClickToTeleporting();
		void EditCoords();
		void EditTPName();
		void EnterTPName(std::string_view teleport_name);
		void GiveEverything();
		void LootCrate(std::string_view current_tp_name, bool take);
		std::vector<std::string> MergeTeleportNames(std::span<std::string> teleport_names) const;
		void MouseDown() const;
		void MouseUp() const;
		void MoveTo360() const;
		void OpenTP() const;
		std::vector<std::string> RequestUserTPNames();
		void EditAndSaveSettings();
		void SetSettings();
		void TakeEverything();

	public:
		DropMode(const std::filesystem::path& settings_path);
		void EditSettings();
		void StartLooting();

	private:
		DropCoordsPosSettings settings_;
		std::vector<std::string> teleports_name_;
		std::string save_teleport_name_;
		std::filesystem::path path_settings_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

		clicker::LeftMouseClicker left_button_;
	};
} // namespace drop_mode