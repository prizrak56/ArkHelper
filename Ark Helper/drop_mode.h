#pragma once
#include <string>
#include "utils.h"
#include "clicker.h"

class DropMode {
public:

	void SupplyCreate();

private:
	void MouseDown();
	void OpenTp();
	void SearchClick(Point search_window);
	void EnterTpName(std::string& teleport_name);
	void ClickToTpName(Point tp_name_s);
	void ClickToTeleporting(Point teleporting);
	void MouseUp();
	void MoveTo360();
	void TakeAll(Point take_all);
	void GiveAll(Point give_all);
	void CloseInventory(Point close_invetory);
	void LootingCrete(std::string& teleport_name, bool take, Point search_window, Point tp_name_s, Point teleporting, Point take_all, Point give_all, Point close_invetory);

	Point ParseButtonCoords(const std::string& input);
	void GetCursorPositionDrop(Point& coords) noexcept;

	LeftMouseClicker left_button_;

	std::string path_to_file_with_coords_ = "C:\\ArkHelper\\drop.txt";
	std::string path_to_the_file_with_the_names_of_teleports_ = "C:\\ArkHelper\\teleports_name.txt";
	std::string path_to_the_file_with_the_name_of_save_teleports_ = "C:\\ArkHelper\\save_teleport.txt";
};