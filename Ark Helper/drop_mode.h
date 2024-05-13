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
	void SearchClick(Point search_window);
	void EnterTpName(std::string& teleport_name);
	void ClickToTpName(Point tp_name_s);
	void ClickToTeleporting(Point teleporting);
	void MouseUp();
	void MoveTo360();
	void TakeAll(Point take_all);
	//void GiveAll();
	void CloseInventory(Point close_invetory);
	void LootingCrete(std::string& current_tp_name, bool take);

	void SetSettings();
	void EditCoords();
	void EditTpName();

public:
	void GiveAll();
	void EditSettings();

	DropMode(std::string& settings_path);
	void StartLooting();

private:

	DropCoordsPosSettings settings_;
	std::vector<std::string> teleports_name_;
	std::string save_teleport_name_;

	LeftMouseClicker left_button_;

	std::string path_settings_ = "C:\\ArkHelper\\settings.txt";

	std::string path_to_file_with_coords_ = "C:\\ArkHelper\\drop.txt";
	std::string path_to_the_file_with_the_names_of_teleports_ = "C:\\ArkHelper\\teleports_name.txt";
	std::string path_to_the_file_with_the_name_of_save_teleports_ = "C:\\ArkHelper\\save_teleport.txt";
};