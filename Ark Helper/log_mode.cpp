#include <fstream>
#include <iostream>

#include "log_mode.h"
#include "string_processing.h"
#include "utils.h"
#include "image_processing.h"

using namespace std::literals;

LogMode::LogMode(const std::filesystem::path& image_save_path) : image_save_path_(image_save_path / "log.png"_p) {}

void LogMode::ChooseOptionsAndStart() {
    system("CLS");

    // the everytime variable is responsible for the minutes after which the image will be sent to discord if the keywords were found (kill or destroyed)
    int everyone_time_counter = 0;

    // the permanentScreenshot variable was not named entirely correctly; it is responsible for sending messages to discord if keywords were not found
    int not_everyone_time_counter = 0;

    std::cout << "enter @everyone time delay(minutes): "s;
    std::cin >> everyone_time_counter;
    std::cout << "indicate the time of sending any screenshot (minutes): "s;
    std::cin >> not_everyone_time_counter;
    system("cls");

    // Measurements of cutting the screen
    int x_screen_cutter = 0;
    int y_screen_cutter = 0;
    int width = 0;
    int height = 0;

    std::cout << "F1 - Start 1920x1080\n" << "F2 - Start 2048x1080\nF3 - Start 2048x1080(not tested)\nF4 = Start 3840x2160(not tested)\nF5 - Start FullScreen(There may be false positives)\n"s;

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
        Sleep(20);
    }
    NoticeUser(everyone_time_counter, not_everyone_time_counter);
}

void LogMode::NoticeUser(int& everyone_time_counter,int& not_everyone_time_counter) {

    DiscordWebhook discord_webhook;

    discord_webhook.SendText("Start...");

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
        std::cout << "F2 - disable\n";
        if (screen_resolution != ScreenResolution::FULL_SCREEN) {
            CaptureScreen(image_save_path_.string().c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width, screen_coords_.height);
        }
        else {
            CaptureScreen(image_save_path_.string().c_str(), screen_coords_.x, screen_coords_.y, screen_coords_.width_full, screen_coords_.height_full);
        }

        std::string newStringProc = ReadImage(image_save_path_);

        bool is_kill_or_destroyed_detected = SplitIntoWordsAndFindWord(newStringProc);

        if (is_kill_or_destroyed_detected && everyone_time_counter <= everyone_timer_duration && is_kill_or_destroyed_detected == true) {
            SendDiscordMessageAndRestartTimerCustomPosScreenResolution(discord_webhook, everyone_timer_start, everyone_timer_duration, is_kill_or_destroyed_detected);
        }
        else if (not_everyone_timer_duration >= not_everyone_time_counter) {
            SendDiscordMessageAndRestartTimerCustomPosScreenResolution(discord_webhook, not_everyone_timer_start, not_everyone_timer_duration,false);
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

void LogMode::EditScreenSettings(int x, int y, int width, int height){
    screen_coords_.x = x;
    screen_coords_.y = y;
    screen_coords_.height = height;
    screen_coords_.width = width;
    mode_selection.first = Command::LOG_MODE;
    mode_selection.second = Command::START;
}

void LogMode::SendDiscordMessageAndRestartTimerCustomPosScreenResolution(DiscordWebhook& discord_webhook, std::chrono::steady_clock::time_point& reset_timer_start, int& reset_timer_duration, bool is_everyone){

    if (is_everyone == true) {
        discord_webhook.SendText("@everyone");
    }
    switch (screen_resolution){

    case ScreenResolution::FULL_SCREEN:
        discord_webhook.SendImage(screen_coords_.x, screen_coords_.y, screen_coords_.width_full, screen_coords_.height_full, image_save_path_);
        reset_timer_start = std::chrono::steady_clock::now();
        reset_timer_duration = 0;
        break;
    default:
        discord_webhook.SendImage(screen_coords_.x, screen_coords_.y, screen_coords_.width, screen_coords_.height, image_save_path_);
        reset_timer_start = std::chrono::steady_clock::now();
        reset_timer_duration = 0;
        break;
    }
}