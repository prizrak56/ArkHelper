#include <fstream>
#include <vector>

#include "webhook.h"

#include <atlimage.h> // for CImage
#include <opencv2/opencv.hpp>

namespace webhook {
    using namespace std::literals;

// ------------------------------- [Discord Webhook] Realization ----------------------
//                                                                                    +
//                                                                                    + ----------------------------------
// ------------------------------------------------------------------------------------ Auxiliary Entities & Constructor +

    std::filesystem::path operator""_p(const char* ch, std::size_t size) {
        return std::filesystem::path(ch, ch + size);
    }

    DiscordWebhook::DiscordWebhook() {
        SetWebhook();
    }

    DiscordWebhook::DiscordWebhook(std::string webhook_path) : webhook_path_(webhook_path) {
        SetWebhook();
    }

//                                                                                    + ---------------------------
// ------------------------------------------------------------------------------------ Discord Webhook Interface +

    void DiscordWebhook::CraftCommand(const std::string& message) {
        std::string command = cmd_1_for_text_message_ + message + cmd_2_for_text_message_ + GetWebHookUrl().string();
        SendDiscordMessage(command);
    }

    const std::filesystem::path DiscordWebhook::GetWebHookUrl() const {
        return webhook_path_;
    }

    int DiscordWebhook::SendDiscordMessage(std::string& command) noexcept {
        command.insert(0, "/C ");

        SHELLEXECUTEINFOA sh_exec_info = { 0 };
        sh_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
        sh_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;
        sh_exec_info.hwnd = NULL;
        sh_exec_info.lpVerb = NULL;
        sh_exec_info.lpFile = "cmd.exe";
        sh_exec_info.lpParameters = command.c_str();
        sh_exec_info.lpDirectory = NULL;
        sh_exec_info.nShow = SW_HIDE;
        sh_exec_info.hInstApp = NULL;

        if (ShellExecuteExA(&sh_exec_info) == FALSE) {
            return -1;
        }

        if (sh_exec_info.hProcess != nullptr) {
            WaitForSingleObject(sh_exec_info.hProcess, INFINITE);
            DWORD rv;
            GetExitCodeProcess(sh_exec_info.hProcess, &rv);
            CloseHandle(sh_exec_info.hProcess);

            return rv;
        }

        return -1;
    }

    void DiscordWebhook::SendImage(int x, int y, int width, int height, const std::filesystem::path& image_path) {

        HBITMAP h_bitmap = processing::CaptureScreenshotForWebhook(x, y, width, height);
        std::vector<BYTE> buf;
        IStream* stream = NULL;
        HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
        CImage image;
        image.Attach(h_bitmap);
        image.Save(stream, Gdiplus::ImageFormatPNG);
        ULARGE_INTEGER liSize;
        IStream_Size(stream, &liSize);
        DWORD len = liSize.LowPart;
        IStream_Reset(stream);
        buf.resize(len);
        IStream_Read(stream, &buf[0], len);
        stream->Release();

        std::fstream fi;
        fi.open(image_path, std::fstream::binary | std::fstream::out);
        fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
        fi.close();

        // Отправка изображения в Discord через вебхук
        // Sending an image to Discord via webhook
        const std::string command = cmd_for_image_message_ + image_path.string() + "\" " + GetWebHookUrl().string() + " > NUL";
        system(command.c_str());

        DeleteObject(h_bitmap); // Освобождение ресурсов битмапа || Releasing bitmap resources
    }

//                                                                                    + ---------------------------------
// ------------------------------------------------------------------------------------ Discord Webhook Private Methods +

    void DiscordWebhook::SetWebhook() {

        std::ifstream input(webhook_path_, std::ios::in);

        if (!input.is_open()) {
            std::cout << webhook_path_ << " file was not created or opened"s << std::endl;
            return;
        }

        std::string line;
        std::size_t counter = 0;

        while (std::getline(input, line)) {
            if (counter == 1) {
                webhook_path_ = line;
                input.close();
                return;
            }

            ++counter;
        }
        input.close();
    }
} // namespace webhook