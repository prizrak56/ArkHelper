#pragma once
#include <windows.h>
#include <string>

class FarmMode {
public:

    FarmMode(HWND h_window_handle);

    FarmMode(std::string file_with_setting, HWND& h_window_handle);

    void FarmStart();

private:

    void ToggleResource(bool& resource);

    void ApplyOrInitializeSettingsFromFile();

    void GetSetCursorPosition(int& x1, int& y1, int& x2, int& y2);

    void Drop(std::string name);

    void HandleInventory();

    void EditCoordsPos();

    void ClickToSearchInventory();

    void ClickToDrop();

    HWND h_window_handle_ = nullptr;

    Point search_button_;
    Point drop_button_;

    const std::string stone_ = "STONE";
    const std::string berry_ = "BERRY";
    const std::string flint_ = "FLINT";
    const std::string wood_ = "WOOD";
    const std::string thatch_ = "THATCH";
    const std::string sand_ = "SAND";
    const std::string metal_ = "METAL";

    std::string search_drop_button_file_path_ = "C:\\ArkHelper\\farm resources.txt";

    bool b_stone_ = true;
    bool b_berry_ = true;
    bool b_flint_ = true;
    bool b_wood_ = true;
    bool b_thatch_ = true;
    bool b_sand_ = true;
    bool b_metal_ = false;
};
