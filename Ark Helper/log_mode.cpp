#include <iostream>
#include <stdexcept>

#include "image_processing.h"
#include "log_mode.h"
#include "string_processing.h"
#include "utils.h"

namespace processing {
    // ---------------------------------- [Log Mode] Realization --------------------------
    //                                                                                    +
    //                                                                                    + ----------------------------------
    // ------------------------------------------------------------------------------------ Auxiliary Entities & Constructor +
    using namespace std::literals;

    std::filesystem::path operator""_p(const char* ch, std::size_t size) {
        return std::filesystem::path(ch, ch + size);
    }

    LogMode::LogMode(const std::filesystem::path& image_save_path) : image_save_path_(image_save_path / "log.png"_p) {}

    //
    // 
    //                                                                                    + --------------------
    // ------------------------------------------------------------------------------------ Log Mode Interface +

    void LogMode::EditScreenSettings(int x, int y, int width, int height) {
        screen_coords_.x = x;
        screen_coords_.y = y;
        screen_coords_.height = height;
        screen_coords_.width = width;
        mode_selection.first = Command::LOG_MODE;
        mode_selection.second = Command::START;
    }

    void LogMode::NoticeUser(int everyone_time_counter, int not_everyone_time_counter) {
        DiscordWebhook discord_webhook;
        discord_webhook.SendText("Start..."s);

        //timer 1
        auto everyone_timer_start = std::chrono::steady_clock::now();
        auto everyone_timer_end = std::chrono::steady_clock::now();
        auto everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(everyone_timer_end - everyone_timer_start).count();

        //timer 2
        auto not_everyone_timer_start = std::chrono::steady_clock::now();
        auto not_everyone_timer_end = std::chrono::steady_clock::now();
        auto not_everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(not_everyone_timer_end - not_everyone_timer_start).count();

        while (mode_selection.second == Command::START) {

            system("cls");
            std::cout << "F2 - disable"s << std::endl;
            std::string image_path = image_save_path_.string();

            if (screen_resolution != ScreenResolution::FULL_SCREEN) {
                CaptureScreen(image_path.c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width, screen_coords_.height);
            }
            else {
                CaptureScreen(image_path.c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width_full, screen_coords_.height_full);
            }

            std::string newStringProc = ReadImage(image_save_path_);
            bool is_kill_or_destroyed_detected = SplitIntoWordsAndFindWord(newStringProc);

            if (everyone_time_counter <= everyone_timer_duration && is_kill_or_destroyed_detected == true) {
                SendDiscordMessageAndRestartTimer(discord_webhook, everyone_timer_start, everyone_timer_duration, is_kill_or_destroyed_detected);
            }
            else if (not_everyone_timer_duration >= not_everyone_time_counter) {
                SendDiscordMessageAndRestartTimer(discord_webhook, not_everyone_timer_start, not_everyone_timer_duration, false);
            }

            while (mode_selection.second == Command::START && everyone_timer_duration < everyone_time_counter && not_everyone_timer_duration < not_everyone_time_counter) {
                everyone_timer_end = std::chrono::steady_clock::now();
                everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(everyone_timer_end - everyone_timer_start).count();

                not_everyone_timer_end = std::chrono::steady_clock::now();
                not_everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(not_everyone_timer_end - not_everyone_timer_start).count();

                if (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {
                    break;
                }

                Sleep(200);
            }
        }

        system("cls");
        MenuMessage();
    }

    void LogMode::SendDiscordMessageAndRestartTimer(
        DiscordWebhook& discord_webhook,
        std::chrono::steady_clock::time_point& reset_timer_start,
        int& reset_timer_duration,
        bool is_everyone) {

        if (is_everyone == true) {
            discord_webhook.SendText("@everyone"s);
        }

        switch (screen_resolution) {
        case ScreenResolution::FULL_SCREEN:
            discord_webhook.SendImage(screen_coords_.x, screen_coords_.y, screen_coords_.width_full, screen_coords_.height_full, image_save_path_);
            break;

        default:
            discord_webhook.SendImage(screen_coords_.x, screen_coords_.y, screen_coords_.width, screen_coords_.height, image_save_path_);
            break;
        }

        reset_timer_start = std::chrono::steady_clock::now();
        reset_timer_duration = 0;
    }

    //
    // 
    //                                                                                    + --------------------------
    // ------------------------------------------------------------------------------------ Log Mode Private Methods +

    void LogMode::ChooseOptionsAndStart() {
        system("CLS");

        // The everytime variable is responsible for minutes after which the image will be sent to Discord 
        // if the keywords were found, killed or destroyed
        int everyone_time_counter = 0;

        // The not_everyone variable is responsible for sending messages to discord if the keywords were not found
        int not_everyone_time_counter = 0;

        std::cout << "Enter @everyone time delay (minutes): "s;
        std::cin >> everyone_time_counter;
        std::cout << "Indicate the time of sending any screenshot (minutes): "s;
        std::cin >> not_everyone_time_counter;
        system("cls");

        // Parameters of cutting the screen
        int x_screen_cutter = 0;
        int y_screen_cutter = 0;
        int width = 0;
        int height = 0;

        std::cout
            << "F1 - Start 1920x1080"s << std::endl
            << "F2 - Start 2560x1440"s << std::endl
            << "F3 - Start 2048x1080 (not tested)"s << std::endl
            << "F4 = Start 3840x2160 (not tested)"s << std::endl
            << "F5 - Start FullScreen (there may be false triggers)"s << std::endl;

        mode_selection.second = Command::SELECT_SETTINGS;

        while (mode_selection.second == Command::SELECT_SETTINGS) {

            switch (screen_resolution) {
            case ScreenResolution::S_1920x1080:
                Beep(500, 800);
                EditScreenSettings(750, 120, 400, 635);
                break;

            case ScreenResolution::S_2560x1440:
                Beep(500, 800);
                EditScreenSettings(1000, 170, 600, 950);
                break;

            case ScreenResolution::S_2048x1080:
                Beep(500, 800);
                EditScreenSettings(800, 120, 430, 635);
                break;

            case ScreenResolution::S_3840x2160:
                Beep(500, 800);
                EditScreenSettings(1500, 240, 800, 1260);
                break;

            case ScreenResolution::FULL_SCREEN:
                Beep(500, 800);
                EditScreenSettings(1, 1, 0, 0);
                break;
            }
            Sleep(200);
        }
        NoticeUser(everyone_time_counter, not_everyone_time_counter);
    }
} // namespace processing


//<<<<<<< Updated upstream
//=======
//void LogMode::NoticeUser(int& everyone_time_counter,int& not_everyone_time_counter) {
//
//    DiscordWebhook discord_webhook;
//
//    discord_webhook.SendText("Start...");
//
//    //timer 1
//    auto everyone_timer_start = std::chrono::steady_clock::now();
//    auto everyone_timer_end = std::chrono::steady_clock::now();
//    auto everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(everyone_timer_end - everyone_timer_start).count();
//
//    //timer 2
//    auto not_everyone_timer_start = std::chrono::steady_clock::now();
//    auto not_everyone_timer_end = std::chrono::steady_clock::now();
//    auto not_everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(not_everyone_timer_end - not_everyone_timer_start).count();
//
//    while (mode_selection.second == Command::START) {
//
//        system("cls");
//        std::cout << "F2 - disable\n";
//        if (screen_resolution != ScreenResolution::FULL_SCREEN) {
//            image_processing::CaptureScreen(image_save_path_.string().c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width, screen_coords_.height);
//        }
//        else {
//            image_processing::CaptureScreen(image_save_path_.string().c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width_full, screen_coords_.height_full);
//        }
//
//        std::string newStringProc = image_processing::ReadImage(image_save_path_);
//
//        bool is_kill_or_destroyed_detected = SplitIntoWordsAndFindWord(newStringProc);
//
//        if (is_kill_or_destroyed_detected && everyone_time_counter <= everyone_timer_duration && is_kill_or_destroyed_detected == true) {
//            SendDiscordMessageAndRestartTimerCustomPosScreenResolution(discord_webhook, everyone_timer_start, everyone_timer_duration, is_kill_or_destroyed_detected);
//        }
//        else if (not_everyone_timer_duration >= not_everyone_time_counter) {
//            SendDiscordMessageAndRestartTimerCustomPosScreenResolution(discord_webhook, not_everyone_timer_start, not_everyone_timer_duration,false);
//        }
//
//        while (mode_selection.second == Command::START && everyone_timer_duration < everyone_time_counter && not_everyone_timer_duration < not_everyone_time_counter) {
//
//            everyone_timer_end = std::chrono::steady_clock::now();
//            everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(everyone_timer_end - everyone_timer_start).count();
//
//            not_everyone_timer_end = std::chrono::steady_clock::now();
//            not_everyone_timer_duration = std::chrono::duration_cast<std::chrono::minutes>(not_everyone_timer_end - not_everyone_timer_start).count();
//
//            if (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {
//                break;
//>>>>>>> Stashed changes
//            }
//
//            Sleep(20);
//        }
//
//        NoticeUser(everyone_time_counter, not_everyone_time_counter);
//    }