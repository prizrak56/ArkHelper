#pragma once
#include <windows.h>
#include <string>

class FarmMode {
public:

    FarmMode();

    FarmMode(std::string file_with_setting);

    void FarmStart();

private:

    void ToggleResource(bool resource);

    void ApplyOrInitializeSettingsFromFile();

    bool FileExists();

    void ClearFile() const;

    void GetSetCursorPosition(int& x1, int& y1, int& x2, int& y2);

    void Drop(std::string name);

    void HandleInventory();

    void EditCoordsPos();

    void ClickToSearchInventory();

    void ClickToDrop();

    HWND hWindowHandle_;

    int search_x_ = 0;
    int search_y_ = 0;
    int drop_x_ = 0;
    int drop_y_ = 0;

    const std::string stone_ = "STONE";
    const std::string berry_ = "BERRY";
    const std::string flint_ = "FLINT";
    const std::string wood_ = "WOOD";
    const std::string thatch_ = "THATCH";
    const std::string sand_ = "SAND";
    const std::string metal_ = "METAL";

    std::string search_drop_button_file_path_ = "C:\\ArkHelper\\for_farm.txt";

    bool b_stone_ = true;
    bool b_berry_ = true;
    bool b_flint_ = true;
    bool b_wood_ = true;
    bool b_thatch_ = true;
    bool b_sand_ = true;
    bool b_metal_ = false;
};
