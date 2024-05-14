#pragma once
#include <string>
#include <vector>
#include "utils.h"
#include "clicker.h"



class DropMode {
private:

	struct DropCoordsPosSettings {
		Point search_window;
		Point first_tp_name_in_list;
		Point teleporting;
		Point take_all;
		Point give_all;
		Point close_invetory;
	};

	void MouseDown();
	void OpenTp();
	void SearchClick();
	void EnterTpName(std::string& teleport_name);
	void ClickToTpName();
	void ClickToTeleporting();
	void MouseUp();
	void MoveTo360();
	void TakeAll();
	void GiveAll();
	void CloseInventory();
	void LootingCrete(std::string& current_tp_name, bool take);

	void SetSettings();
	void EditCoords();
	void EditTpName();

public:

	void EditSettings();
	DropMode(const std::filesystem::path& settings_path);
	void StartLooting();

private:

	DropCoordsPosSettings settings_;

	std::vector<std::string> teleports_name_;

	std::string save_teleport_name_;

	LeftMouseClicker left_button_;

	std::filesystem::path path_settings_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

};