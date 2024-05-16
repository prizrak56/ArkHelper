#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <Windows.h>

namespace farm {
// ----------------------------------- [Farm Mode] Definition -------------------------
//                                                                                    +
//                                                                                    + --------------------
// ------------------------------------------------------------------------------------ Auxiliary Entities +

    using namespace std::literals;
    std::filesystem::path operator""_p(const char* ch, std::size_t size);

// 
// 
//                                                                                    + -----------
// ------------------------------------------------------------------------------------ Farm Mode +

    class FarmMode {
    private:
        void ApplyOrInitializeSettingsFromFile();
        void ClickToDrop();
        void ClickToSearchInventory();
        void Drop(std::string_view name);
        void HandleInventory();
        void SelectResources(int first_timer, int second_timer);
        void ToggleResource(bool& resource);

    public:
        FarmMode(HWND h_window_handle);
        FarmMode(const std::filesystem::path& file_with_setting, HWND& h_window_handle);
        void EditCoords();
        void StartFarm();
        
    private:
        HWND h_window_handle_ = nullptr;
        Point search_button_;
        Point drop_button_;

        const std::string stone_ = "STONE"s;
        const std::string berry_ = "BERRY"s;
        const std::string flint_ = "FLINT"s;
        const std::string wood_ = "WOOD"s;
        const std::string thatch_ = "THATCH"s;
        const std::string sand_ = "SAND"s;
        const std::string metal_ = "METAL"s;

        std::filesystem::path path_settings_ = "C:\\"_p / "ArkHelper"_p / "settings.txt"_p;

        bool b_stone_ = true;
        bool b_berry_ = true;
        bool b_flint_ = true;
        bool b_wood_ = true;
        bool b_thatch_ = true;
        bool b_sand_ = true;
        bool b_metal_ = false;
    };
} // namespace farm